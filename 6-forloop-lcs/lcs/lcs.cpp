#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include "omploop.hpp"

#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif
int max(int a, int b)
{
    return (a > b) ? a : b;
}


int main (int argc, char* argv[]) {

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }
  OmpLoop loop;
  int m = atoi(argv[1]);
  int n = atoi(argv[2]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  
  //insert LCS code here.
    int L[m + 1][n + 1];
    int i, j;
  
    //Following steps build L[m+1][n+1] in bottom up fashion. Note 
    //that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1]
    loop.parfor(0, n*m, 1,
S        [&](float& tls) {tls=L;}, 
	[&](int x, float& tls) -> void{
	i = x/n;
	j = x%n;
            if (i == 0 || j == 0)
                tls[i][j] = 0;
  
            else if (X[i - 1] == Y[j - 1])
                tls[i][j] = tls[i - 1][j - 1] + 1;
  
            else
                tls[i][j] = max(tls[i - 1][j], tls[i][j - 1]);
        }, 
	[&](float& tls) {L=tls;}
    );
  
    //L[m][n] contains length of LCS for X[0..n-1] and Y[0..m-1]
  int result = L[m][n]; // length of common subsequence


  checkLCS(X, m, Y, n, result);

}

/*
#include <bits/stdc++.h>
  
int max(int a, int b);
  
// Returns length of LCS for X[0..m-1], Y[0..n-1]
int lcs(char* X, char* Y, int m, int n)
{
    int L[m + 1][n + 1];
    int i, j;
  
    //Following steps build L[m+1][n+1] in bottom up fashion. Note 
    //that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1]
    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                L[i][j] = 0;
  
            else if (X[i - 1] == Y[j - 1])
                L[i][j] = L[i - 1][j - 1] + 1;
  
            else
                L[i][j] = max(L[i - 1][j], L[i][j - 1]);
        }
    }
  
    //L[m][n] contains length of LCS for X[0..n-1] and Y[0..m-1]
    return L[m][n];
}
  
// Utility function to get max of 2 integers
int max(int a, int b)
{
    return (a > b) ? a : b;
}
  
// Driver program to test above function 
int main()
{
    char X[] = "AGGTAB";
    char Y[] = "GXTXAYB";
  
    int m = strlen(X);
    int n = strlen(Y);
  
    printf("Length of LCS is %d\n", lcs(X, Y, m, n));
  
    return 0;
}
*/
