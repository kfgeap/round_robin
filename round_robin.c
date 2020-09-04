#include <stdio.h>
#include <stdlib.h>

/////プロトタイプ宣言/////
void sort(int *q);

/////メイン関数/////
int main(void)
{

    /////宣言部/////
    FILE *fp;                       //ファイルポインタ
    char fname[] = "processes.csv"; //読み込むファイル名
    float cpuTime;                  //タイムクウォンタム
    int n;                          //プロセス数
    float *needTimes;               //プロセス実行時間格納用
    float *insertTimes;             //プロセス開始時間格納用
    int *queue;                     //実行待ち行列
    int qn = 0;                     //実行待ち行列管理用（最後尾+1の場所を示す）
    float time = 0;                 //現在の時刻

    /////ファイル読み込み処理/////
    fp = fopen(fname, "r");
    if (fp == NULL)
    {
        printf("%s の読み込みに失敗しました。\n", fname);
        return -1;
    }

    fscanf(fp, "%d,%f", &n, &cpuTime);

    needTimes = (float *)malloc(sizeof(float) * n); //メモリ領域の確保
    insertTimes = (float *)malloc(sizeof(float) * n);
    queue = (int *)malloc(sizeof(int) * n);
    if ((needTimes == NULL) || (insertTimes == NULL))
    {
        printf("メモリ確保に失敗しました。\n");
        return -1;
    }
    else
    {
        printf("メモリを確保しました。\n");
        printf("プロセス数：%d  タイムクウォンタム：%f\n", n, cpuTime);
    }

    printf("-----------------------------------------------\n");
    printf("プロセスID  プロセス実行時間：プロセス開始時間\n");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < n; i++)
    { //確保した配列にデータを代入
        fscanf(fp, "%f,%f", &needTimes[i], &insertTimes[i]);
        printf("%d  %f：%f\n", i, needTimes[i], insertTimes[i]);
    }
    printf("-----------------------------------------------\n");
    printf("ファイルを読み込みました。\n");

    /////ラウンドロビンスケジューリング実行部/////
    printf("\nプロセスの実行を開始します。\n");
    //実行待ち行列に初期値を挿入
    for (int i = 0; i < n; i++)
    {
        if (insertTimes[i] == 0)
        {
            queue[qn] = i;
            qn++;
        }
    }
    //実行待ち行列の空きスロットに-1を挿入
    for (int i = qn; i < n; i++)
    {
        queue[i] = -1;
    }

    printf("初期状態の実行待ち行列：");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", queue[i]);
    }
    printf("\n\n");

    //処理開始
    while (queue[0] != -1)
    {
        printf("現在時刻：%f\n", time);
        printf("プロセス%dを実行します：%f → ", queue[0], needTimes[queue[0]]);
        needTimes[queue[0]] -= cpuTime;
        printf("%f\n", needTimes[queue[0]]);
        time += cpuTime;

        if (needTimes[queue[0]] <= 0)
        {
            printf("プロセス%d実行終了\n", queue[0]);
            for (int i = 0; i < n - 1; i++)
            {
                queue[i] = queue[i + 1];
            }
            queue[n - 1] = -1;
            qn--;
        }
        else
        {
            printf("プロセス%dを実行待ち行列の最後尾に挿入します。\n", queue[0]);
            int temp = queue[0];
            for (int i = 0; i < n - 1; i++)
            {
                queue[i] = queue[i + 1];
            }
            queue[qn - 1] = temp;
        }

        printf("現在の実行待ち行列：");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", queue[i]);
        }
        printf("\n");

        //プロセスの発生時間をチェック
        for (int i = 0; i < n; i++)
        {
            if ((insertTimes[i] > time - cpuTime) && (insertTimes[i] <= time))
            {
                printf("プロセス%dを実行待ち行列に挿入します。\n", i);
                queue[qn] = i;
                qn++;
            }
        }

        printf("\n");
    }

    printf("全てのプロセスを実行しました。\n");
    printf("終了時刻：%f\n", time);

    //ファイルを閉じる
    fclose(fp);
    //メモリ開放
    free(needTimes);
    free(insertTimes);
    free(queue);

    return 0;
}