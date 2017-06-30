#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <iostream>
#include <time.h>  
#include <stdlib.h>
using namespace std;
int random(int min, int max)
{
   static bool first = true;
   if ( first ) 
   {  
      srand(time(NULL)); 
      first = false;
   }
   return min + rand() % (max - min);
}


void * get_in_addr(struct sockaddr * sa)
{
	if(sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr); 
	}
	
	return &(((struct sockaddr_in6 *)sa)->sin6_addr); 
}

int main(int argc, char * argv[])
{int status;
	struct addrinfo hints, * res;
	int listner; 
	
	
	 
	memset(& hints, 0, sizeof hints);
	// Set the attribute for hint
	hints.ai_family = AF_UNSPEC; // We don't care V4 AF_INET or 6 AF_INET6
	hints.ai_socktype = SOCK_STREAM; // TCP Socket SOCK_DGRAM 
	hints.ai_flags = AI_PASSIVE; 
	
	 
	status = getaddrinfo(NULL, "8916" , &hints, &res);
	if(status != 0)
	{
		fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
	}
	
	
	listner = socket(res->ai_family,res->ai_socktype, res->ai_protocol);
	if(listner < 0 )
	{
		fprintf(stderr,"socket error: %s\n",gai_strerror(status));
	}
	
	 
	status = bind(listner, res->ai_addr, res->ai_addrlen); 
	if(status < 0)
	{
		fprintf(stderr,"bind: %s\n",gai_strerror(status));
	}

	status = listen(listner, 10); 
	if(status < 0)
	{
		fprintf(stderr,"listen: %s\n",gai_strerror(status));
	}
	
	// Free the res linked list after we are done with it	
	freeaddrinfo(res);
	
	
	// We should wait now for a connection to accept
	int new_conn_fd;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	char s[INET6_ADDRSTRLEN]; // an empty string 
		
	// Calculate the size of the data structure	
	addr_size = sizeof client_addr;
	
// getting input
    int i,j,k,l;

    //Get Frame
    int fs;
    cout<<"\n Enter Frame size: ";
    cin>>fs;

    int f[20];

    cout<<"\n Enter Frame:";
    for(i=0;i<fs;i++)
    {
        cin>>f[i];
    }

    //Get Generator
    int gs;
  //  cout<<"\n Enter Generator size: ";
    //cin>>gs;
    gs=4;
    int g[20];

    cout<<"\n Enter Generator:";
    for(i=0;i<gs;i++)
    {
        //cin>>g[i];
    }
    g[0]=1;
    g[1]=0;
    g[2]=1;
    g[3]=1;
    cout<<"\n Sender Side:";
    cout<<"\n Frame: ";
    for(i=0;i<fs;i++)
    {
        cout<<f[i];
    }
    cout<<"\n Generator :";
    for(i=0;i<gs;i++)
    {
        cout<<g[i];
    }

    //Append 0's
    int rs=gs-1;
    cout<<"\n Number of 0's to be appended: "<<rs;
    for (i=fs;i<fs+rs;i++)
    {
        f[i]=0;
    }

    int temp[20];
    for(i=0;i<20;i++)
    {
        temp[i]=f[i];
    }

    cout<<"\n Message after appending 0's :";
    for(i=0; i<fs+rs;i++)
    {
        cout<<temp[i];
    }

    //Division
    for(i=0;i<fs;i++)
    {
        j=0;
        k=i;
        //check whether it is divisible or not
        if (temp[k]>=g[j])
        {
            for(j=0,k=i;j<gs;j++,k++)
            {
                if((temp[k]==1 && g[j]==1) || (temp[k]==0 && g[j]==0))
                {
                    temp[k]=0;
                }
                else
                {
                    temp[k]=1;
                }
            }
        }
    }
      cout<<endl<<"now"<<endl;
     for(i=0; i<fs+rs;i++)
    {
        cout<<temp[i];
    }

    cout<<"\n Reaminder: ";

    //CRC
    int crc[15];
    for(i=0,j=fs;i<rs;i++,j++)
    {
        crc[i]=temp[j];
    }

    cout<<"\n CRC bits: ";
    for(i=0;i<3;i++)
    {
        cout<<crc[i];
    }

    cout<<"\n Transmitted Frame: ";
    int tf[15];
    for(i=0;i<fs;i++)
    {
        tf[i]=f[i];
    }
    for(i=fs,j=0;i<fs+rs;i++,j++)
    {
        tf[i]=crc[j];
    }
    for(i=0;i<fs+rs;i++)
    {
        cout<<tf[i];
    }
    cout<<"bit stuffing\n";
    int tfbs[25];
    int ctr=0;
    int jt=0,nsb=0,nvb=0;
    for(int i=0;i<fs+rs;++i)
    {

    if(tf[i]==1)
            {
                    if(ctr==3)
                    {
                        tfbs[jt]=0;
                        jt++;
                        tfbs[jt]=tf[i];
                        ++nsb;
                        jt++;
                        ctr=1;
                    }
                     else{               
                            tfbs[jt]=tf[i];
                            ++jt;
                            ++ctr;
                        }
            }
    else
        {
                ctr=0;
                tfbs[jt]=tf[i];
                ++jt;
        }
    }
    nvb=jt-nsb;
    for(i=0;i<jt;i++)
    {
        cout<<tfbs[i]<<" ";
    }
    cout<<endl;
    cout<<"adding flag\n";
    int tffl[100];
    for(int i=0;i<4;++i)
        tffl[i]=1;
    int fct=4;
    for(int i=0;i<jt;++i)
    {
        tffl[fct]=tfbs[i];
        ++fct;
    }
    for(int i=0;i<4;++i)
        {
        tffl[fct]=1;
        ++fct;
        }
    for(i=0;i<fct;i++)
    {
        cout<<tffl[i]<<" ";
    }
    cout<<endl;
    cout<<"no of stuffed bits="<<nsb<<endl;
    cout<<"no of valid bits="<<nvb<<endl;

cout<<"tampering\n";
int taw;
 for(int i=0;i<(fct-8)/4;++i)
    {taw=random(0,fct+5);
    if(taw<fct)
    {   if(taw>fct/2)
        {
            i--;
        }
        else{
        if(tffl[taw]==0)
            tffl[taw]=1;
        else
            tffl[taw]=0;
        }
    }
    }
//embedding
printf("input.\n");
	char c[100];
  for(int i=0;i<100;++i)
    c[i]='a'; 
	int ctre;
	c[0]='A'+fct;
	c[1]='A'+nvb;
	c[2]='A'+nsb;
	for(int i=3;i<fct+3;++i)
    {
        //cout<<"enter"<<endl;
        ctre=tffl[i-3];
        c[i]='A'+ctre;
        cout<<ctre<<" ";//<<endl;
    }



printf("I am now accepting connections ...\n");
    int kx=0;
	while(kx<3){
		// Accept a new connection and return back the socket desciptor 
		new_conn_fd = accept(listner, (struct sockaddr *) & client_addr, &addr_size);	
		if(new_conn_fd < 0)
		{
			fprintf(stderr,"accept: %s\n",gai_strerror(new_conn_fd));
			continue;
		}
	
		inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr),s ,sizeof s); 
		printf("I am now connected to %s \n",s);
		status = send(new_conn_fd,c,100,0);
		if(status == -1)
		{
			close(new_conn_fd);
			_exit(4);
		}
		++kx;
		
	}
	// Close the socket before we finish 
	close(new_conn_fd);	
	
	return 0;

}
