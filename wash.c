#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utime.h>
#include <time.h>

int main(int argc, char *argv[])
{
 
 
  if(argc <= 1)
  {
    printf("No files input \n");
    exit(-1);
  }
  
  char command = 'j';

  printf("Enter in one of the following commands:");
  printf(" ’c’ list the current file to /dev/tty\n");
  printf(" ’d’ copy the current file (duplicate)\n");
  printf(" ’r’ rename the current file\n");
  printf(" ’u’ delete (unlink) the current file\n"); 
  printf(" ’t’ zero (length) the current file\n");
  printf(" ’a’ append the current file to another file\n");
  printf(" ’l’ display the last 100 bytes of the current file\n");
  printf(" ’m’ change the current file’s access permissions\n"); 
  printf(" ’x’ change the current file’s access time to the current time \n");
  printf(" ’n’ goto the next name (i.e., the next file)\n");
  printf(" ’q’ quit\n");
         
  
  for (int i = 1; i < argc; i++)
  {
    //printf("This is file number %d\n", i);
    FILE *fp;
    printf("File: %s", argv[i]);
    if(fp == NULL)
    {
      perror("open");
    }
    printf(" Command: ");
    scanf(" %c", &command);
    while(command != 'n')
    {
      if (command == 'c')
      {
  	fp = fopen(argv[i], "r");
        char c;
        c = fgetc(fp);
        while(c != EOF)
        {
	  printf("%c", c);
          c = fgetc(fp);
      	}
      }

      if (command == 'd')
      {
        fp = fopen(argv[i], "r");
        if(fp == NULL)
        {
          perror("open");
        }
        char ch;
        char filename[101];
        FILE *newfile;  
 
        printf("Enter in file destination: ");
	scanf("%s", filename); 

        newfile = fopen(filename, "w");
        while((ch = fgetc(fp)) != EOF)
        {
	  fputc(ch, newfile);
        }
        fclose(newfile); 
        fclose(fp);
        printf("File has been duplicated.\n");
      }

      if (command == 'r')
      { 
        fp = fopen(argv[i], "r");
        if(fp == NULL)
        {
          perror("open");
        }
        char newname[101];
        printf("Enter in new name: \n");
        scanf("%s", newname);
       
        int r = rename(argv[i], newname);
        argv[i] = newname;
        if(r == 0)
	  printf("File successfully renamed\n");
        else
          printf("Error in renaming\n");
        
        
        fclose(fp);
      }

      if (command == 'u')
      {
        fp = fopen(argv[i], "r");
        if(fp == NULL)
        {
          perror("open");
        }
        int ret;
        ret = remove(argv[i]);
        if(ret == 0)
        {
          printf("File deleted.\n");
          break;
        }
        else
        {
          printf("File not deleted.\n");
	}
        fclose(fp);
      }

      if (command == 't')
      {
        int fd = open(argv[i], O_WRONLY|O_TRUNC);
        if(fd < 0)
        {
          perror("open");
        }
        if (close(fd) < 0)
    	  perror("close");
        printf("File has been truncated.\n");
      }

      if (command == 'a')
      {
        fp = fopen(argv[i], "r");
        if(fp == NULL)
        {
          perror("open");
        }
        char ch;
        char filename[101];
        FILE *newfile;  
 
        printf("Enter in file destination: ");
	scanf("%s", filename); 

        newfile = fopen(filename, "a");
        while((ch = fgetc(fp)) != EOF)
        {
	  fputc(ch, newfile);
        }
        printf("File appended to: %s\n", filename); 
        fclose(newfile); 
        fclose(fp);
        
      }

      if (command == 'l')
      {
        char c[101];
        int fp = open(argv[i], O_RDONLY);
        if(fp < 0)
        {
          perror("open");
        }
        lseek(fp, -100L, SEEK_END);
        int filebytes = read(fp, c, 100);
        c[filebytes] = '\0';

        printf("%s", c);

        if (close(fp) < 0)
    	  perror("close");
      }

      if (command == 'm')
      { 
        char mode[15];
        printf("Type what permission to change: \n");
        scanf("%s", mode);
        
        if(strcmp(mode, "700") == 0)
        {
          chmod(argv[i], S_IRUSR | S_IWUSR | S_IXUSR);
          printf("Changed mode.\n");
        }
        else if(strcmp(mode, "644") == 0)
        {
          chmod(argv[i], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
          printf("Changed mode.\n");
        }
        else if(strcmp(mode, "777") == 0)
        {
	  chmod(argv[i], S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);  
          printf("Changed mode.\n");
        }
        else if(strcmp(mode, "666") == 0)
        {
	  chmod(argv[i], S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	  printf("Changed mode.\n"); 
        }
        else if(strcmp(mode, "755") == 0)
        {
	  chmod(argv[i], S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); 
	  printf("Changed mode.\n"); 
        } 
        else if(strcmp(mode, "600") == 0)
        {
          chmod(argv[i], S_IRUSR | S_IWUSR);
          printf("Changed mode.\n");
        }
        else
        {
	  printf("Here are the commands you can enter: 777, 755, 700, 666, 644, 600\n");
        }
      }

      if (command == 'x')
      {
        struct stat s;
        struct utimbuf t;

        if((fp = fopen(argv[i], "r+b")) < 0)
        {
          printf("Error opening file\n");
          return -1;
        }
        if(stat(argv[i], &s) < 0)
        {
          printf("Error getting file file stats\n");
          return -1;
        }

       
        fclose(fp);

        if(utime(argv[i], &t) < 0)
        {
          printf("unable to reset time\n");
          return -1;
        }

        printf("Here is the time: %s", ctime(&s.st_ctime));

      }

      if (command == 'q')
      {
        exit(-1);

      }
      printf("File: %s", argv[i]);
      printf(" Command: ");
      scanf(" %c", &command);
    }
  }


 return 0;


}

