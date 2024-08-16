#include "bits/stdc++.h"
#include "PINYIN.h"
#include "font.h"

using namespace std;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef unsigned short uint16_t;


FILE *file = NULL;

void openFile(const char *filename) {
    // �Զ����ƶ�дģʽ���ļ�
    file = fopen(filename, "wb");
    if (file == NULL) {
        printf("OPEN FAILED %s\n", filename);
        return;
    }
}

void closeFile() {
    // �ر��ļ�
    if (file != NULL) {
        fclose(file);
    }
}

void writeUint8ToFile(int position, uint8_t value) {
    // ����ļ��Ƿ��Ѵ�
    if (file == NULL) {
        printf("NOT OPEN\n");
        return;
    }

    // �ƶ���ָ��λ��
    fseek(file, position, SEEK_SET);

    // д��uint8ֵ
    fwrite(&value, sizeof(uint8_t), 1, file);

//    printf("WRITE  %d TO %d\n", value, position);
}

int formatInt(int number) {
    int formatted = number;
    int length = 0;
    int temp = number;

    // ����������λ��
    while (temp != 0) {
        temp /= 10;
        length++;
    }
    // ���λ������6λ�����ں��油0
    if (length < 6) {
        for (int i = 0; i < 6 - length; ++i) {
            formatted *= 10;
        }
    }
    return formatted;
}

char num_excel[8][5] = {
        "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
};

int get_num(const char *a) {
    int num = 0;
    int bin = 100000;
    for (int j = 0; j < strlen(a); j++) {
        int now_num = 0;
        for (int i = 0; i < 8; ++i) {
            for (int k = 0; k < strlen(num_excel[i]); ++k) {
                if (num_excel[i][k] == a[j]) {
                    now_num = i + 2;
                    goto end_loop;
                }
            }
        }
        end_loop:
        num += bin * now_num;
        bin /= 10;
    }
    return num;
}
bool judge_belong(int a,int b)
{
    for (int i = 100000; i >=1 ; i/=10) {
        if(a/i==0)break;
        if(a/i!=b/i)return 0;
        a=a-a/i*i;
        b=b-b/i*i;

    }
    return 1;
}

int binarySearch(int target, int *not_found) {
    int left = 0;
    int right = 213;
    *not_found = 1; // ��ʼ�趨δ�ҵ�

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int mid_num = pinyin_all[mid].code;

        if (mid_num == target) {
            *not_found = 0; // �ҵ���
            return mid;
        } else if (target < mid_num) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    // �Ҳ���Ŀ��ֵ�����ر�Ŀ��ֵ��һ����ֵ
    if (left <= 213) {
        int mid_num = pinyin_all[left].code;
        if (judge_belong(target, mid_num)) {

            return left;
        }

    }

    return -1;
}


int main() {
    printf("%d\n", judge_belong(666000, 666400));

    return 0;
        FILE *file_printf;
    file_printf = fopen("output.txt", "wb"); // ���ļ��Թ�д��

    if (file_printf == NULL) {
        printf("�޷����ļ�\n");
        return 1;
    }


    const char filename[] = "../PINYIN.BIN";
    // ���ļ�
    openFile(filename);
    char num_excel[1000000] = {0};
    int max_num = 0;
    for (int i = 0; i < 399; ++i) {
        num_excel[get_num(pin[i].pinyin)]++;
        if (num_excel[get_num(pin[i].pinyin)] > max_num)max_num = num_excel[get_num(pin[i].pinyin)];
    }
    fprintf(file_printf, "%d 55555555555\n", max_num);
    int youxiao_cnt = 0;
    for (int i = 0; i < 1000000; ++i) {

        if (num_excel[i]) {
            //����
            writeUint8ToFile(youxiao_cnt * 128, (i & 0xff));
            writeUint8ToFile(youxiao_cnt * 128 + 1, (i & 0xff00) >> 8);
            writeUint8ToFile(youxiao_cnt * 128 + 2, (i & 0xff0000) >> 16);
            writeUint8ToFile(youxiao_cnt * 128 + 3, (i & 0xff000000) >> 24);
            //ƴ������
            writeUint8ToFile(youxiao_cnt * 128 + 4, num_excel[i]);
            int pinyin_cnt = 1;
            fprintf(file_printf, "{%d,%d,{", i, num_excel[i]);

            for (int j = 0; j < 399; ++j) {
                if (get_num(pin[j].pinyin) == i) //���ƴ������
                {
                    fprintf(file_printf, "{\"%6s\"},", pin[j].pinyin);

                    //д��ƴ��
                    for (int k = 0; k < 6; k++)
                        writeUint8ToFile(youxiao_cnt * 128 + pinyin_cnt * 16 + k, pin[j].pinyin[k]);
                    //д������
                    writeUint8ToFile(youxiao_cnt * 128 + pinyin_cnt * 16 + 6, pin[j].num);
                    //д���ַ
                    writeUint8ToFile(youxiao_cnt * 128 + pinyin_cnt * 16 + 7, (pin[j].add & 0xff));
                    writeUint8ToFile(youxiao_cnt * 128 + pinyin_cnt * 16 + 8, (pin[j].add & 0xff00) >> 8);
                    writeUint8ToFile(youxiao_cnt * 128 + pinyin_cnt * 16 + 9, (pin[j].add & 0xff0000) >> 16);
                    writeUint8ToFile(youxiao_cnt * 128 + pinyin_cnt * 16 + 10, (pin[j].add & 0xff000000) >> 24);
                    pinyin_cnt++;
                }
            }
            fprintf(file_printf, "}},\n", i);

            youxiao_cnt++;
        }
    }
    for (int i = 0; i < 399; i++) {
        for (int j = 0; j < strlen(name[i]); ++j) {
            writeUint8ToFile(pin[i].add - 0x20000 + j, name[i][j]);

        }
    }
    writeUint8ToFile(0xa31f, 0);

    closeFile();
    fclose(file_printf); // �ر��ļ�




    while (1) {
        int input;
        scanf("%d", &input);
        int flag;
        int look = binarySearch(formatInt(input), &flag);
        if (look != -1) {
            if (flag) { //û�ҵ�
                int cnt=0;
                for (int j = look; j<214 ; j++) {

                    if(judge_belong(formatInt(input),pinyin_all[j].code))
                     {
                         printf("%d %d\n",look,j);
                        for (int i = 0; i < pinyin_all[j].num; ++i) {
                            cnt++;
                            printf("%d :%s ", cnt, pinyin_all[j].str[i]);
                        }
                        printf("\n");
                    }else break;
                }

            } else {
//                printf("%d:\n", pinyin_all[look].num);
                for (int i = 0; i < pinyin_all[look].num; ++i) {
                    printf("%d :%s ", i, pinyin_all[look].str[i]);
                }
                printf("\n");
            }
        }
    }
    return 0;

}
