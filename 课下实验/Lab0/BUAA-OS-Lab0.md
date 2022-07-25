# BUAA-OS-Lab0

## Exercise0.1

### 1.补全palindrome.c

 在lab0工作区的src目录中，存在一个名为palindrome.c的文件，使用刚刚学过的工具打开palindrome.c，使用c语言实现判断输入整数n(1≤n≤10000)是否为回文数的程序(输入输出部分已经完成)。通过stdin每次只输入一个整数n，若这个数字为回文数则输出Y并退出程序，否则输出N并退出程序。**[注意：正读倒读相同的整数叫回文数]**

```c
//palindrome.c
#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);

        int temp1=0,temp2=n;
	while(n>0){
		temp1*=10;
		temp1+=n%10;
		n/=10;
	}
	if(temp1==temp2){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}
```

### 2.补全Makefile完成编译链接

在src目录下，存在一个未补全的Makefile文件，借助刚刚掌握的Makefile知识，将其补全，以实现通过make指令触发src目录下的palindrome.c文件的编译链接的功能，生成的可执行文件命名为palindrome

Makefile

```makefile
all: palindrome.c
	gcc palindrome.c -o palindrome
```



### 3.补全.sh完成提取文件对应行数

在src/sh_test目录下，有一个file文件和hello_os.sh文件。hello_os.sh是一个未完成的脚本文档，请同学们借助shell编程的知识，将其补完，以实现通过指令bash hello_os.sh AAA BBB，在hello_os.sh所处的文件夹新建一个名为BBB的文件，其内容为AAA文件的第8、32、128、512、1024行的内容提取(AAA文件行数一定超过1024行)。**[注意：对于指令bash hello_os.sh AAA BBB，AAA及BBB可为任何合法文件的名称，例如bash hello_os.sh file hello_os.c，若以有hello_os.c文件，则将其原有内容覆盖]**

```shell
#hello_os.sh
#!/bin/bash
head -n 8 $1 | tail -n 1 > $2
head -n 32 $1 | tail -n 1 >> $2
head -n 128 $1 | tail -n 1 >> $2
head -n 512 $1 | tail -n 1 >> $2
head -n 1024 $1 | tail -n 1 >> $2
# sed -n "5p" file
```

或使用for循环控制

```shell
#!/bin/bash
sed -n "8p" $1 > $2
for number in 32 128 512 1024;do
        sed -n "${number}p" $1 >> $2
done
```

### 4.复制

将补全后的palindrome.c、Makefile、hello_os.sh依次复制到路径/dst/palindrome.c   /dst/Makefile   /dst/sh_test/hello_os.sh **[注意：文件名和路径必须与题目要求相同]**

![pic_1](https://raw.githubusercontent.com/Cptzzzzz/image/image/pic_1.jpg)

## Exercise0.2

### 1.shell脚本

在lab0工作区的ray/sh_test1目录中，含有100个子文件夹file1~file100，还存在一个名为changefile.sh的文件，将其补完，以实现通过指令bash changefile.sh，可以删除该文件夹内file71-file100共计30个子文件夹，将file41~file70共计30个子文件夹重命名为newfile41-newfile70，保留file1-file40共计40个子文件夹。**[注意：评测时仅评测changefile.sh的正确性]**

```shell
#!/bin/bash
a=1
while [ $a -le 100 ]
do
        if [ $a -gt 70 ]           #if loop variable is greater than 70
        then
                final_file_name="file"$((a))
                rm -r $final_file_name
        elif [ $a -gt 40 ]         # else if loop variable is great than 40   
        then
                final_file_name="newfile"$((a))
                origin_file_name="file"$((a))
                mv $origin_file_name $final_file_name
        fi
                   #don't forget change the loop variable
                 a=`expr  $a + 1`
done
```



## Exercise0.3

### 1.grep搜索、管道以及awk处理文本

在lab0工作区的ray/sh_test2目录下，存在一个未补全的search.sh文件，将其补完，以实现通过指令bash search.sh file int result，可以在当前文件夹下生成result文件，内容为file文件所有含有int字符串对应的行数，即若有多行含有int字符串需要全部输出。**[注意：对于指令bash search.sh file int result，file及result可为任何合法文件名称，int可为任何合法字符串，若已有result文件，则将其原有内容覆盖，匹配时大小写不忽略]**

   结果**文件内容格式**应如下图所示：

![pic_2](https://raw.githubusercontent.com/Cptzzzzz/image/image/pic_2.png)

```shell
grep int file -n | awk -F: '{print $1}' > output
```

## Exercise0.4

### 1.替换文件中文本

在lab0工作区的csc/code目录下，存在fibo.c、main.c，其中fibo.c有点小问题，还有一个未补全的modify.sh文件，将其补完，以实现通过指令bash modify.sh fibo.c char int，可以将fibo.c中所有的char字符串更改为int字符串。**[注意：对于指令bash modify.sh fibo.c char int，fibo.c可为任何合法文件，char及int可为任何合法字符串，评测时评测modify.sh的正确性，而不是检查修改后fibo.c的正确性]**

```shell
sed -i s/int/char/g 
sed s/原文本/替换后文本/g -i
-i指对原文本操作
```

### 2.补全Makefile，实现gcc的编译、链接以及头文件目录的指定

lab0工作区的csc/code/fibo.c成功更换字段后(bash modify.sh fibo.c char int)，现已有csc/Makefile和csc/code/Makefile，补全两个Makefile文件，要求在csc目录下通过指令make可在csc/code文件夹中生成fibo.o、main.o，在csc文件夹中生成可执行文件fibo，再输入指令make clean后只删除两个.o文件。 [ **注意：不能修改fibo.h和main.c文件中的内容，提交的文件中fibo.c必须是修改后正确的fibo.c，可执行文件fibo作用是从stdin输入一个整数n，可以输出斐波那契数列前n项，每一项之间用空格分开。比如n=5，输出1 1 2 3 5** ]

   make后、make clean后(csc文件夹内)应如下图从左到右所示(**最终提交时文件中fibo和.o文件可有可无**)： 

![pic_3](https://raw.githubusercontent.com/Cptzzzzz/image/image/pic_3.jpg)

![pic_4](https://raw.githubusercontent.com/Cptzzzzz/image/image/pic_4.jpg)

Makefile:

```makefile
all:
	cd code&&make
clean:
	rm code/main.o
	rm code/fibo.o
```

code/Makefile

```makefile
all:
	gcc -c fibo.c -o fibo.o -I ../include
	gcc -c main.c -o main.o -I ../include
	gcc main.o fibo.o -o ../fibo
```

