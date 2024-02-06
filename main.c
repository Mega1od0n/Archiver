#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create(char *file1, int argc, char *argv[]) {
    FILE *arc = fopen(file1, "wb");
    int size[128];
    fprintf(arc, "%d\n", argc - 4);
    for (int i = 0; i < argc - 4; i++)
    {
        FILE *file2 = fopen(argv[i + 4], "rb");
        if (file2 == NULL)
        {
            printf("File %s does not open\n", argv[i + 4]);
            continue;
        }
        fseek(file2, 0, SEEK_END);
        size[i] = ftell(file2);
        fseek(file2, 0, SEEK_SET);
        fprintf(arc, "%s - %d\n", argv[i + 4], size[i]);
        fclose(file2);
    }

    for (int i = 0; i < argc - 4; i++)
    {
        FILE *file2 = fopen(argv[i + 4], "rb");
        if (file2 != NULL)
        {
            printf("File %s successfully added to an archive\n", argv[i + 4]);
        }
        char* temp = malloc(size[i] * sizeof(char));
        fread(temp, 1, size[i], file2);
        fwrite(temp, 1, size[i], arc);
        fclose(file2);
    }
    fclose(arc);
}

void list(char *file) {
    FILE *arc = fopen(file, "rb");
    char temp[128];
    char count[10], *end_;
    fscanf(arc, "%s", count);
    long long cnt = strtol(count, &end_, 10);
    for (int i = 0; i < cnt; i++)
    {
        fscanf(arc, "%s", temp);
        printf("%s\n", temp);
        fscanf(arc, "%s", temp);
        fscanf(arc, "%s", temp);
    }
    fclose(arc);
}


void extract(char *file) {
    FILE *arc = fopen(file, "rb");
    char count[10], *end;
    fscanf(arc, "%s", count);
    long long cnt = strtol(count, &end, 10);
    int names_start = ftell(arc);
    char cur_file_name[128] = {0};
    char dump[128] = {0};
    char temp[128];
    for (int i = 0; i < cnt; i++)
    {
        fscanf(arc, "%s", temp);
        fscanf(arc, "%s", temp);
        fscanf(arc, "%s", temp);
    }
    int cur_file_size, content_start = ftell(arc), end_;
    fseek(arc, names_start, SEEK_SET);
    for (int i = 0; i < cnt; i++)
    {
        fscanf(arc, "%s%s%llu", cur_file_name, dump, &cur_file_size);
        end_ = ftell(arc);
        FILE *cur_file = fopen(cur_file_name, "wb");
        fseek(arc, content_start + 1, SEEK_SET);
        char* temp = malloc(cur_file_size * sizeof(char*));
        fread(temp, 1, cur_file_size, arc);
        fwrite(temp, 1, cur_file_size, cur_file);
        content_start += cur_file_size;
        fseek(arc, end_, SEEK_SET);
        fclose(cur_file);
    }
    printf("Completed!\n");
    fclose(arc);
}

int main(int argc, char *argv[]) {
    char *arc;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp("--file", argv[i])==0)
        {
            arc = argv[i + 1];
        }
        else if (strcmp("--create", argv[i])==0)
        {
            create(arc, argc, argv);
            break;
        }
        else if (strcmp("--extract", argv[i])==0)
        {
            extract(arc);
        }
        else if (strcmp("--list", argv[i])==0)
        {
            list(arc);
        }
    }
}
