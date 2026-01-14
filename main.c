#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

struct Student{
    char student_id[20];
    char student_name[50];
    int minor_marks[5];
    int major_marks[5];
    int total_marks;
};

int validate_studentID(char id[]){
    for(int i=0;id[i]!='\0';i++){
        if(!isalnum(id[i]))
            return 0;
    }
    return 1;
}
int is_duplicate(struct Student *stu[],int count,char id[]){
    for(int i=0;i<count;i++){
        if(strcmp(stu[i]->student_id,id)==0)
            return 1;
    }
    return 0;
}
int validate_name(char name[]){
    for(int i=0;name[i]!='\0';i++){
        if(!isalpha(name[i])) 
            return 0;
    }
    return 1;
}
char* compute_grade(int marks){
    if(marks>=90)
        return "O";
    else if(marks>=85)
        return "A+";
    else if(marks>=75)
        return "A";
    else if(marks>=65)
        return "B+";
    else if(marks>=60)
        return "B";
    else if(marks>=55)
        return "C";
    else if(marks>=50)
        return "D";
    else
        return "F";
}
void skip_line(FILE *fp){
    int ch;
    while((ch=fgetc(fp))!='\n'&&ch!=EOF);
}

int main(int argc,char *argv[]){
    if(argc<2){
        printf("Provide name of Input file\n");
        return 1;
    }
    int n,valid_count=0;
    FILE *input_file=fopen("file.txt","r");
    FILE *output_file=fopen("output.txt","w");
    FILE *error_file=fopen("error.txt","w");
    if(input_file==NULL){
        printf("Error in opening input file\n");
        return 0;
    }
    if(output_file==NULL||error_file==NULL){
        printf("Error in opening output or error file\n");
        return 0;
    }
    fscanf(input_file,"%d",&n);
    struct Student *stu[n];
    for(int i=0;i<n;i++){
        stu[valid_count]=malloc(sizeof(struct Student));
        if(stu[valid_count]==NULL){
            break;
        }
        fscanf(input_file,"%s",stu[valid_count]->student_id);
        if(!validate_studentID(stu[valid_count]->student_id)){
            fprintf(error_file,"Student ID %s is Invalid\n",stu[valid_count]->student_id);
            free(stu[valid_count]);
            skip_line(input_file);
            continue;
        }
        if(is_duplicate(stu,valid_count,stu[valid_count]->student_id)){
            fprintf(error_file,"Duplicate Student ID %s found(First occurence considered)\n",stu[valid_count]->student_id);
            free(stu[valid_count]);
            skip_line(input_file);
            continue;
        }
        fscanf(input_file,"%s",stu[valid_count]->student_name);
        if(!validate_name(stu[valid_count]->student_name)){
            fprintf(error_file,"Student Name %s is Invalid\n",stu[valid_count]->student_name);
            free(stu[valid_count]);
            skip_line(input_file);
            continue;
        }
        int mark_error=0;
        stu[valid_count]->total_marks=0;
        for(int j=0;j<5;j++){
            fscanf(input_file,"%d %d",&stu[valid_count]->minor_marks[j],&stu[valid_count]->major_marks[j]);
            if(stu[valid_count]->minor_marks[j]<0||stu[valid_count]->minor_marks[j]>40||stu[valid_count]->major_marks[j]<0||stu[valid_count]->major_marks[j]>60){
                fprintf(error_file,"Subject %d Marks out of range for Student ID:%s\n",i,stu[valid_count]->student_id);
                mark_error=1;
                break;
            }
        }
        if(mark_error){
            free(stu[valid_count]);
            skip_line(input_file);
            continue;
        }
        valid_count++;
    }
    fclose(input_file);
    fprintf(output_file,"Student ID\tName\tSubject 1\tSubject 2\tSubject 3\tSubject 4\tSubject 5\tTotal Marks\tPercentage\n");
    fprintf(output_file,"```````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````\n");
    for(int i=0;i<valid_count;i++){
        fprintf(output_file,"%s\t%s\t",stu[i]->student_id,stu[i]->student_name);
        for(int j=0;j<5;j++){
            stu[i]->total_marks=stu[i]->total_marks+stu[i]->minor_marks[j]+stu[i]->major_marks[j];
            fprintf(output_file,"%s\t\t",compute_grade(stu[i]->minor_marks[j]+stu[i]->major_marks[j]));
        }
        fprintf(output_file,"%d\t\t%.2f \t",stu[i]->total_marks,stu[i]->total_marks/5.0);
        fprintf(output_file,"\n");
    }
    printf("Saved Student Report to output.txt and error logs to error.txt\n");
    for(int i=0;i<valid_count;i++)
        free(stu[i]);
    return 0;
}
