
//  

#include<cstdio>
#include"TwoDimArray.hpp"
#include"SlideBlock.hpp"

using namespace std;

const int NCOL = 5;
const int NROW = 9;

int main(void){

	//-------------------------------------------------------------------
	// Sample of Two Dim Array
	//-------------------------------------------------------------------
	TwoDimArray<int> test(NROW, NCOL);
	printf("Sample of Two Dim Array: (9 rows) x (5 columns) matrix\n");

	// insert sample data
	for(int i = 0; i < NROW; i++)
		for(int j = 0; j < NCOL; j++)
			test(i,j) = ( i + 1 ) * 10 + ( j + 1 );

	// print sample data
	printf("X (row_index, col_index)\n");
	for(int i = 0; i < test.get_nrow(); i++){
		for(int j = 0; j < test.get_ncol(); j++){
			printf("X(%d,%d)=%d ", i,j,test(i,j) );
		}
		printf("\n");
	}
	printf("\n");

	printf("X[col_index][row_index]\n");
	for(int i = 0; i < test.get_nrow(); i++){
		for(int j = 0; j < test.get_ncol(); j++){
			printf("X[%d][%d]=%d ", j,i,test[j][i] );
		}
		printf("\n");
	}
	printf("\n");

	//-------------------------------------------------------------------
	// Sample of FIFO_slideBlock
	//-------------------------------------------------------------------
	FIFO_SlideBlock<double> test2(NROW, NCOL);
	printf("Sample of FIFO Slide Block\n");

	// insert sample data
	for(int j = 0; j <NCOL; j++){
		for(int i = 0; i < NROW; i++){
			test2.new_frame_ptr()[i] = (double)(j + 1) / (i + 1) ;
		}
		test2.next_frame();
		printf("pushing data. Current ncol is %d\n", test2.get_ncol() );
	}
	printf("\n");

	// print sample data
	printf("Initial data\n");
	for(int i = 0; i < test2.get_nrow(); i++){
		for(int j = 0; j < test2.get_ncol(); j++){
			printf("%5.3f\t", test2(i,j));
		}
		printf("\n");
	}
	printf("\n");

	//-------------------------------------------------------------------
	// Sample of FIFO_slideBlock.kill_frame
	//-------------------------------------------------------------------

	// kill
	test2.kill_frame();
	printf("Killed one frame (column)\n");

	// print sample data
	for(int i = 0; i < test2.get_nrow(); i++){
		for(int j = 0; j < test2.get_ncol(); j++){
			printf("%5.3f\t", test2(i,j));
		}
		printf("\n");
	}
	printf("\n");


	// kill twice
	test2.kill_frame();
	test2.kill_frame();
	printf("Killed two frames (columns)\n");

	// print sample data
	for(int i = 0; i < test2.get_nrow(); i++){
		for(int j = 0; j < test2.get_ncol(); j++){
			printf("%5.3f\t", test2(i,j));
		}
		printf("\n");
	}
	printf("\n");


	//-------------------------------------------------------------------
	// Sample of FIFO_slideBlock push data
	//-------------------------------------------------------------------

	// push
	for(int i = 0; i < test2.get_nrow(); i++){
		test2.new_frame_ptr()[i] = 1.111 * ( i + 1 );
	}
	test2.next_frame();
	printf("Pushed one frame (column)\n");

	// print sample data
	for(int i = 0; i < test2.get_nrow(); i++){
		for(int j = 0; j < test2.get_ncol(); j++){
			printf("%5.3f\t", test2(i,j));
		}
		printf("\n");
	}
	printf("\n");


	//-------------------------------------------------------------------
	// Sample of FIFO_slideBlock.rewind_frame
	//-------------------------------------------------------------------

	// rewind frame
	test2.rewind_frame();
	printf("Rewinded the sliding block\n");

	// print sample data
	for(int i = 0; i < test2.get_nrow(); i++){
		for(int j = 0; j < test2.get_ncol(); j++){
			printf("%5.3f\t", test2(i,j));
		}
		printf("\n");
	}
	printf("\n");

	//-------------------------------------------------------------------
	// Sample of FIFO_slideBlock : difference between get_ncol and get_max_ncol
	//-------------------------------------------------------------------
	printf("get_ncol(): %d \t get_max_ncol(): %d \n", test2.get_ncol(), test2.get_max_ncol() );

	return 0;
}

