#include<iostream>
#include<omp.h>
#include<stdlib.h>
#include<time.h>

using namespace std;
 
int main()
{
    int a[200][200],b[200][200],c[200][200],m=200,n=200,p=200,q=200,i,j,k,N=200,s=4;
    int no_threads[]={1,2,4,6,8,10,12,14,18,22,26,30,34,38,42,46,50,54,58,62};
    int th;
    int temp;
    //cout<<"\nEnter first matrix:\n";
    for(i=0;i<m;++i)
        for(j=0;j<n;++j)
            a[i][j]=5;
    //cout<<"\nEnter second matrix:\n";
    for(i=0;i<p;++i)
        for(j=0;j<q;++j)
            b[i][j]=7;
    //cout<<"\nThe new matrix is:\n";
    //for(th=19;th>=0;th--)
    //{
    	double start = omp_get_wtime();
    	#pragma omp parallel //for num_threads(no_threads[th]) 
    	{
            #pragma omp for
    	    for(int jj=0;jj<N;jj+= s){
                for(int kk=0;kk<N;kk+= s){
                    for(int i=0;i<N;i++){
                        for(int j = jj; j<((jj+s)>N?N:(jj+s)); j++){
                            temp = 0;
                            for(int k = kk; k<((kk+s)>N?N:(kk+s)); k++){
                                    temp += a[i][k]*b[k][j];
                            }
                            c[i][j] += temp;
                        }
                    }
                }
            }
    	}
    	double end = omp_get_wtime();
    	double time_elapsed = end - start;
    	cout<<time_elapsed<<endl;

    //}
    
    return(0);
}
