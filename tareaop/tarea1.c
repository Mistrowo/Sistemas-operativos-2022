
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
  


//En este ejemplo se generan 3 hijos del mismo proceso padre y asu vez nietos
int main()
{


//Variables a utilizar  donde se usa un arreglo de palabras para mandar a las pipes, esto lo controla el proceso central
// las variables aux son las pipes
// las variables fds seran nuestros procesos de escritura y lectura

   int fds[2], fds2[2],fds3[2],fds4[2];
   int aux,aux3;
   char mensajes[2][100]={"Hola", "Hello"};
   char lectura[1000];
   aux = pipe(fds);
   aux3 = pipe(fds3);
   int aux4=pipe(fds4);
   int aux1=pipe(fds2);
   int pid, pid1, pid2,pid3,pid4,pid5;
 
   if (aux || aux1 || aux4== -1) {

      printf("No se leyo nada\n");
      return 1;
   }
   
    
// los sleep aplicados son para ordenar la estructura del chat, en otras palabras para que no salgan 
//desordenados los procesos.
    pid = fork();
  
    
    if (pid == 0) {
        //Usuario 2 que a su vez tiene 2  hijos


//En este Punto se manda el proceso bi direccional del hijo 1 escribir y leer
    close(fds[0]);
    write(fds[1], mensajes[0], sizeof(mensajes[0]));
    write(fds[1], mensajes[1], sizeof(mensajes[1]));
    close(fds[1]);


    close(fds2[1]);
    read(fds2[0], lectura, sizeof(lectura));
    printf("Usuario 2: %s  --> pid = %d  les dejo mi padre = %d\n",lectura,getpid(), getppid());   
    exit(9);
    sleep(2);
    close(fds2[0]);



//En este punto el usuario 2 tiene un hijo sin embargo este no se ejecuta pq se sale del chat
// pobrecito nunca se ejecutara ;,(
 pid3=fork();



      if(pid3 == 0){

    close(fds[0]);
    write(fds[1], mensajes[0], sizeof(mensajes[0]));
    write(fds[1], mensajes[1], sizeof(mensajes[1]));
    close(fds[1]);


    close(fds2[1]);
    read(fds2[0], lectura, sizeof(lectura));
    printf("Usuario 4: %s  --> pid = %d  yo tambien les dejo mi padre = %d\n",lectura,getpid(), getppid());   
    exit(9);
    sleep(2);
    close(fds2[0]);
 
      }


    }
  
    else {
        

    //Aquí va el segundo hijo, mismo caso proceso de escritura al central y  lectura
        pid1 = fork();
        
        if (pid1 == 0) {


        close(fds[0]);
        write(fds[1], mensajes[0], sizeof(mensajes[0]));
        write(fds[1], mensajes[1], sizeof(mensajes[1]));
        close(fds[1]);
            
        close(fds3[1]);
        read(fds3[0], lectura, sizeof(lectura));
        printf("Usuario 3:  %s  --> pid = %d  y mi padre es = %d\n",lectura,getpid(), getppid());
        sleep(2) ;  
        close(fds3[0]);


        // el hijo 2 tiene un hijo que poseera las mismas funciones lectura y escritura

        pid4=fork();

        if(pid4==0){


    close(fds[0]);
    write(fds[1], mensajes[0], sizeof(mensajes[0]));
    write(fds[1], mensajes[1], sizeof(mensajes[1]));
    close(fds[1]);


        close(fds3[1]);
        read(fds3[0], lectura, sizeof(lectura));
        printf("Usuario 5:  %s  --> pid = %d  y mi padre es = %d\n",lectura,getpid(), getppid());
        sleep(2);  
        
        close(fds3[0]);

         

        }
            
        }
        else {

//Este es el 3 hijo del proceso central

            pid2 = fork();
            if (pid2 == 0) {


       close(fds[0]);
        write(fds[1], mensajes[0], sizeof(mensajes[0]));
        write(fds[1], mensajes[1], sizeof(mensajes[1]));
        close(fds[1]);

       
        close(fds4[1]);
        read(fds4[0], lectura, sizeof(lectura));
        printf("Usuario 6:  %s  --> pid = %d  y mi padre es = %d\n",lectura,getpid(), getppid());
        close(fds4[0]);

        

// posee un hijo a su vez que cumple las mismas funciones


          pid5=fork();

         if(pid5==0){

    close(fds[0]);
    write(fds[1], mensajes[0], sizeof(mensajes[0]));
    write(fds[1], mensajes[1], sizeof(mensajes[1]));
    close(fds[1]);


        close(fds4[1]);
        read(fds4[0], lectura, sizeof(lectura));
        printf("Usuario 7:  %s  --> pid = %d, padre es  = %d\n",lectura,getpid(), getppid());
        close(fds4[0]);


         }
     
               
            }
  
           
            else {

                //Este código sólo lo ejecutará el Proceso central, aqui se comunica con todos los procesos
                // mediante escritura y lectura ademas de imprimir algunos mensajes.
               
                 printf("Bienvenido al Server\n");
                 sleep(2);
                
            

                close(fds[1]);
                read(fds[0], lectura, sizeof(lectura));
                printf("Soy el proceso central saluden esbirros,%s---->%d\n", lectura,getpid());
                close(fds[0]);

                 close(fds[0]);
                 write(fds[1], mensajes[0], sizeof(mensajes[0]));
                 write(fds[1], mensajes[1], sizeof(mensajes[1]));
                close(fds[1]);
                


                close(fds2[0]);
                write(fds2[1], mensajes[0], sizeof(mensajes[0]));
                close(fds2[1]);

                close(fds2[1]);
                read(fds2[0], lectura, sizeof(lectura));
                printf("Usuario 2: %s \n", lectura);
                printf("Usuario 2: MUERE HIJO!!!!!!!!!!!!!!!!\n");
                close(fds2[0]);


                close(fds3[0]);
                write(fds3[1], mensajes[0], sizeof(mensajes[0]));
                close(fds3[1]);
                 

                close(fds3[1]);
                read(fds3[0], lectura, sizeof(lectura));
                printf("Usuario 3: %s\n", lectura);
                printf("Usuario 3: Ayudante un 70 por favor\n");
                close(fds3[0]);
               

                close(fds4[0]);
                write(fds4[1], mensajes[1], sizeof(mensajes[1]));
                close(fds4[1]);

                close(fds4[1]);
                read(fds4[0], lectura, sizeof(lectura));
                printf("Usuario 6: %s  \n", lectura);
                printf("Usuario 6: apruebo!\n");
                close(fds4[0]);



                close(fds4[0]);
                write(fds4[1], mensajes[1], sizeof(mensajes[1]));
                close(fds4[1]);

                 close(fds4[1]);
                read(fds4[0], lectura, sizeof(lectura));
                printf("Usuario 7: %s  \n", lectura);
                printf("Usuario 7: PUDU SER PEOR \n");
                close(fds4[0]);



// este sleep es para que el printf que viene salga al ultimo
sleep(5);
printf("Presione ENTER PARA SALIR\n");
                   



            }
        }
    }
  
    return 0;
}     