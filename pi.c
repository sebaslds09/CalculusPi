/* 	Copyright 2010 Sebastian Ortiz
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	Author: Sebastian Ortiz Acevedo 
	Date: 4-Sep-2010 
	pi2.c: This program calculate pi using a mathematical serie 
	Euler serie, this uses 2 threads to duplicate the velocity 
	of the calculus. 
*/ 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/wait.h> //Function WIFEXITED 

int main(int argc, char *argv[]){ 
	long double pi=0.0,i=0.0, temp; //pi saves the value of the calculus and i is a counter, temp is a temporary variable 
	unsigned long long int pres=0,den=0; //pres is a number of iterations and den is serie's denominator 
	int pid, status,com[2]; //pid saves the pid of the thread status saves thread's status and com is a pipe 

	pipe(com); //creates a pipe 

	if (argc==1) //if there isn't argument default is 5000 
		pres=5000; 
	else	
		pres=atol(argv[1]); 

	pid=fork(); //creates the thread 
	if (pid<0) 
		perror("Error!: " ); 

	else if(pid==0){ //the child 
		for (i=0.0,den=1;i<=pres;i+=0.0001,den+=4){ //counts of 0.0001 in 0.0001 and only counts the positive serie's numbers 
			pi+=1/(long double)den; 
		} 
		close(com[0]); 
		write(com[1],&pi,sizeof(long double));//write on the pipe the pi's result 
		exit(1); 
	} 
	else{ 
		for (i=0.0,den=3;i<=pres;i+=0.0001,den+=4){ //counts of 0.0001 in 0.0001 and only counts the negative serie's numbers 
			pi-=1/(long double)den; 
		} 
		wait(&status); //saves the thread's status 
		if (WIFEXITED(status)!=0){ //if the child finish normal 
			close(com[1]); 
			read(com[0],&temp,sizeof(long double)); //read the pipe 
			pi=pi+temp; //add pipe's value to the fhater's pi value 
		} 
		else 
			printf("Error!: El proceso hijo ha finalizado inesperadamenten" );	
	} 
	pi=pi*4; //finish the calculus of pi 
	printf("Pi es aprox. : %.25Lgn",pi); 
	return 0; 
}