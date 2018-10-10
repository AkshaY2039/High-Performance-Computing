#include<iostream>
#include<omp.h>
#include<stdlib.h>
#include<time.h>

using namespace std;
 
int main()
{
    int a[200][200],b[200][200],c[200][200],m=200,n=200,p=200,q=200,i,j,k;
    int no_threads[]={1,2,4,6,8,10,12,14,18,22,26,30,34,38,42,46,50,54,58,62};
    int th;
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
    	for(i=0;i<m;++i)
    	{
    		#pragma omp for //num_threads(no_threads[th]) 
    	    for(j=0;j<q;++j)
    	    {
    	        c[i][j]=0;
    	        for(k=0;k<n;++k)
    	            c[i][j]=c[i][j]+(a[i][k]*b[k][j]);
    	        //cout<<c[i][j]<<" ";
    	    }
    	//cout<<"\n";
    	}
    	double end = omp_get_wtime();
    	double time_elapsed = end - start;
    	cout<<time_elapsed<<endl;
    	}
    //}
    
    return 0;
}
