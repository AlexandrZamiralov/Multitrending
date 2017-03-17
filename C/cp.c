
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char ch, source_file[20], target_file[20]; //итератор, 2 массива для хранения имени файла
    FILE *source, *target; //ссылки на 2 файла
    
    printf("Enter name of file to copy\n");
    gets(source_file); //stdin в source_file
    
    source = fopen(source_file, "r"); // открыть файл для чтения
    

    
    printf("Enter name of target file\n");
    gets(target_file);//stdin в target_file
    
    target = fopen(target_file, "w"); // откроыть файл для записи
    
    
    while( ( ch = fgetc(source) ) != EOF ) //пока символ не конец
    fputc(ch, target); //пихаем его в таргетный файл
    
    printf("File copied successfully.\n");
    
    fclose(source);//закроем файлы напоследок
    fclose(target);
    
    return 0;
}
