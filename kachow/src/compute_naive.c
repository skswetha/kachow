#include "compute.h"

// Computes the convolution of two matrices
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  // TODO: convolve matrix a and matrix b, and store the resulting matrix in
  // output_matrix
    if (!a_matrix || !b_matrix || !output_matrix) {
        return -1;}

    uint32_t a_cols = a_matrix->cols;
    uint32_t b_cols = b_matrix->cols;
    uint32_t a_rows = a_matrix->rows;
    uint32_t b_rows = b_matrix->rows;

    uint32_t output_cols = a_cols - b_cols + 1;
    uint32_t output_rows = a_rows - b_rows + 1;

    (*output_matrix) = (matrix_t *) malloc(sizeof(matrix_t));
    (*output_matrix)->cols = output_cols;
    (*output_matrix)->rows = output_rows;
    (*output_matrix)->data = (int32_t*) malloc(output_cols * output_rows * sizeof(int32_t));

// flip matrix b
// horizontally:
    for (uint32_t i = 0; i < b_rows/2; i++) {
        for (uint32_t k = 0; k < b_cols; k++) {
            int32_t temp = b_matrix->data[b_cols*i + k];
            b_matrix->data[b_cols*i + k] = b_matrix->data[b_cols*(b_rows-i-1)+k];
            b_matrix->data[b_cols*(b_rows-i-1)+k] = temp;
        }

    }


// vertically:
    for (uint32_t i = 0; i < b_rows; i++) {
        for (uint32_t k = 0; k < b_cols/2; k++) {
            int32_t temp = b_matrix->data[b_cols*i + k];
            b_matrix->data[b_cols*i + k] = b_matrix->data[b_cols*i + (b_cols-k-1)];
            b_matrix->data[b_cols*i + (b_cols-k-1)] = temp;
        }

    }
// dot product/convolution
    for (uint32_t i = 0; i < output_rows; i++) {
        for (uint32_t k = 0; k < output_cols; k++) {
            int32_t sum = 0;
            for (uint32_t j = 0; j < b_rows; j++) {
                for (uint32_t l = 0; l < b_cols; l++) {
                    int32_t a_val = a_matrix->data[(i+j)*a_cols + (k+l)];
                    int32_t b_val = b_matrix->data[j*b_cols + l];
                    sum+= a_val*b_val;
                }
            }
            (*output_matrix)->data[i*output_cols + k] = sum;
        }
        }
    

    


  return 0;
}

// Executes a task
int execute_task(task_t *task) {
  matrix_t *a_matrix, *b_matrix, *output_matrix;

  char *a_matrix_path = get_a_matrix_path(task);
  if (read_matrix(a_matrix_path, &a_matrix)) {
    printf("Error reading matrix from %s\n", a_matrix_path);
    return -1;
  }
  free(a_matrix_path);

  char *b_matrix_path = get_b_matrix_path(task);
  if (read_matrix(b_matrix_path, &b_matrix)) {
    printf("Error reading matrix from %s\n", b_matrix_path);
    return -1;
  }
  free(b_matrix_path);

  if (convolve(a_matrix, b_matrix, &output_matrix)) {
    printf("convolve returned a non-zero integer\n");
    return -1;
  }

  char *output_matrix_path = get_output_matrix_path(task);
  if (write_matrix(output_matrix_path, output_matrix)) {
    printf("Error writing matrix to %s\n", output_matrix_path);
    return -1;
  }
  free(output_matrix_path);

  free(a_matrix->data);
  free(b_matrix->data);
  free(output_matrix->data);
  free(a_matrix);
  free(b_matrix);
  free(output_matrix);
  return 0;
}
