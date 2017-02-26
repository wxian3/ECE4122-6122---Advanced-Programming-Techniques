// Distributed two-dimensional Discrete FFT transform
// Wenqi Xian
// ECE8893 Project 1


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <signal.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "Complex.h"
#include "InputImage.h"

using namespace std;

void Transform1D(Complex* h, int w, Complex* H)
{
  // Implement a simple 1-d DFT using the double summation equation
  // given in the assignment handout.  h is the time-domain input
  // data, w is the width (N), and H is the output array.
  for (int n = 0; n < w; n++) {
    for (int k = 0; k < w; k++) {
      H[n] = H[n] + Complex(cos(2 * M_PI * n * k / w), - sin(2 * M_PI * n * k / w)) * h[k]; 
    }
    if (fabs(H[n].real) < 1e-10) H[n].real = 0;
    if (fabs(H[n].imag) < 1e-10) H[n].imag = 0;
  }
}

void Transpose(Complex* result, Complex* input, int row, int col)
{
    for (int i = 0; i < row; i++){
       for (int j = 0; j < col; j++) {
           result[j * row + i] = input[i * col + j];
       }
    }
}

void Transform2D(const char* inputFN) 
{ 
  InputImage image(inputFN);  
  int input_width = image.GetWidth();
  int input_height = image.GetHeight();
  Complex* result = new Complex[input_width * input_height];
  Complex* input = image.GetImageData();

  int numCPU, rank, rc;

  MPI_Comm_size(MPI_COMM_WORLD,&numCPU);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  printf ("Number of CPUs = %d My rank = %d\n", numCPU,rank);

  int TasksPerCpu = input_height * input_width / numCPU;
  MPI_Request* send_request = new MPI_Request[numCPU];
  MPI_Request* recv_request = new MPI_Request[numCPU];
  MPI_Request* recv_request_2 = new MPI_Request[numCPU];
  MPI_Status* send_status = new MPI_Status[numCPU];
  MPI_Status* recv_status = new MPI_Status[numCPU];
  MPI_Status* recv_status_2 = new MPI_Status[numCPU];

      if (rank == 0)
        { 
          // First receive
          for (int i = 0; i < TasksPerCpu / input_width; i++) {
            Transform1D(input + i * input_width, input_width, result + i * input_width);
          }

          for (int cpu = 1; cpu < numCPU; cpu++) { 
            rc = MPI_Irecv(result + TasksPerCpu * cpu, TasksPerCpu * sizeof(Complex),
            MPI_CHAR, cpu, 0, MPI_COMM_WORLD, &recv_request[cpu]);
              if (rc != MPI_SUCCESS)
                {
                  cout << "Rank " << rank
                      << " recv failed, rc " << rc << endl;
                  MPI_Finalize();
                  exit(1);
                }
              cout << "Rank 0 receiving from rank " << cpu << endl;
          }
          
          for (int i = 1; i < numCPU; i++) {
            MPI_Wait(&recv_request[i], &recv_status[i]);
          }
          // Save 1Dimage
          cout << "Rank 0 received data from all ranks " << endl;
          image.SaveImageData("MyAfter1D.txt", result, input_width, input_height);
          
          // First transpose
          Complex* transposed = new Complex[input_width * input_height];
          Transpose(transposed, result, input_height, input_width);

          // Complex* transposed_buf = new Complex[TasksPerCpu];
          // Don't know allocate transposed_buf here, 
          // since you are not allowed to change transposed_buf in non-blocking send until you pass wait.
          // If you want to allocate here, use blocking send.
          // First send 
          for (int cpu = 1; cpu < numCPU; cpu++) { 
            /*
            for (int r = 0; r < TasksPerCpu; r++) {
              transposed_buf[r] = transposed[TasksPerCpu * cpu + r];
            }
            */
            // This is valid, since you are using a different buf(pointer) each time.
            rc = MPI_Isend(transposed + TasksPerCpu * cpu, TasksPerCpu * sizeof(Complex), MPI_CHAR, cpu,
                          0, MPI_COMM_WORLD, &send_request[cpu]);
            if (rc != MPI_SUCCESS)
              {
                cout << "Rank " << rank
                    << " send failed, rc " << rc << endl;
                MPI_Finalize();
                exit(1);
              }
              cout << "Rank 0 sending rank " << cpu << endl;
          }
    
          for (int i = 1; i < numCPU; i++) {
            MPI_Wait(&send_request[i], &send_status[i]); 
          }
          cout << "Rank 0 sent data to all ranks"  << endl;

          // CPU0 calc
          Complex* transposed_result = new Complex[input_width * input_height];
          for (int i = 0; i < TasksPerCpu / input_width; i++) {
            Transform1D(transposed + i * input_width, input_width, transposed_result + i * input_width);
          }

          // Second receive
          for (int cpu = 1; cpu < numCPU; cpu++) { 
            rc = MPI_Irecv(transposed_result + TasksPerCpu * cpu, TasksPerCpu * sizeof(Complex),
            MPI_CHAR, cpu, 0, MPI_COMM_WORLD, &recv_request_2[cpu]);
              if (rc != MPI_SUCCESS)
                {
                  cout << "Rank " << rank
                      << " recv failed, rc " << rc << endl;
                  MPI_Finalize();
                  exit(1);
                }
              cout << "Rank 0 receiving from rank " << cpu << " second time" << endl;
          }
          
          for (int i = 1; i < numCPU; i++) {
            MPI_Wait(&recv_request_2[i], &recv_status_2[i]);
          }
          cout << "Rank 0 received data from all ranks second time" << endl;
          
          // Second transpose
          Complex* final = new Complex[input_width * input_height];
          Transpose(final, transposed_result, input_height, input_width);
          // Save image
          image.SaveImageData("MyAfter2D.txt", final, input_width, input_height);


      } else
        { 
          Complex* buf = new Complex[TasksPerCpu];
          for (int r = 0; r < TasksPerCpu; r++) {
            buf[r] = input[TasksPerCpu * rank + r];
          }
          // Each cpu calculates Transform1D of one part
          Complex* cpu_result_1 = new Complex[TasksPerCpu]; 
          for (int i = 0; i < TasksPerCpu / input_width; i++) {
            Transform1D(buf + i * input_width, input_width, cpu_result_1 + i * input_width);
          }
          
          // First send cpu_result_1 to 0
          rc = MPI_Send(cpu_result_1, TasksPerCpu * sizeof(Complex), MPI_CHAR, 0,
                        0, MPI_COMM_WORLD);
          if (rc != MPI_SUCCESS)
            {
              cout << "Rank " << rank
                   << " send failed, rc " << rc << endl;
              MPI_Finalize();
              exit(1);
            }
          cout << "Rank " << rank << " sent to rank " << 0 << endl;

          // to receive the transposed from 0
          Complex* recv_buf = new Complex[TasksPerCpu];
          MPI_Status status;
          rc = MPI_Recv(recv_buf, TasksPerCpu * sizeof(Complex), MPI_CHAR, 0,
                        0, MPI_COMM_WORLD, &status);
          if (rc != MPI_SUCCESS)
            {
              cout << "Rank " << rank
                   << " recv failed, rc " << rc << endl;
              MPI_Finalize();
              exit(1);
            }
          cout << "Rank " << rank << " received from rank " << 0 << endl;

          Complex* cpu_result_2 = new Complex[TasksPerCpu]; 
          for (int i = 0; i < TasksPerCpu / input_width; i++) {
            Transform1D(recv_buf + i * input_width, input_width, cpu_result_2 + i * input_width);
          }

          // Second send cpu_result_2 to 0
          rc = MPI_Send(cpu_result_2, TasksPerCpu * sizeof(Complex), MPI_CHAR, 0,
                        0, MPI_COMM_WORLD);
          if (rc != MPI_SUCCESS)
            {
              cout << "Rank " << rank
                   << " send failed, rc " << rc << endl;
              MPI_Finalize();
              exit(1);
            }
          cout << "Rank " << rank << " sent to rank " << 0 << " seond time" << endl;
            
        }
}

int main(int argc, char** argv)
{
  string fn("Tower.txt"); // default file name
  if (argc > 1) fn = string(argv[1]);  // if name specified on cmd line
  // MPI initialization here
  int rc = MPI_Init(&argc,&argv);
  if (rc != MPI_SUCCESS) {
    printf ("Error starting MPI program. Terminating.\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
  }
  Transform2D(fn.c_str()); // Perform the transform.
  // Finalize MPI here
  MPI_Finalize();
}  
  

  
