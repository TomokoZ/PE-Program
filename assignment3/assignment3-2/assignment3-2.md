# assignment3-2

在使用ecx作为计数器的时候，发现程序的循环被破坏，无法中止，查阅相关资料之后发现printf在调用时即便没有使用ecx，依然有可能会改变其中的值，将他作为一个中间寄存器使用。所以在使用ecx进行循环的时候，需要保护ecx的数据。

![image-20250402131017946](C:\Users\31631\AppData\Roaming\Typora\typora-user-images\image-20250402131017946.png)

于是在没错循环开始之后，将ecx压栈防止调用，在单词循环结束之后才弹出，保护其中的循环数，防止出现上述的死循环情况。

最后在输出的时候，直接把对应的数字变成字符，变成ascii对应的数据。

![image-20250402131250308](C:\Users\31631\AppData\Roaming\Typora\typora-user-images\image-20250402131250308.png)