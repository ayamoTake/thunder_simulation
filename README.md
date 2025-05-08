# 雷のシミュレーション

これだけを観て, 再現を試みました

https://www.youtube.com/watch?v=3mVXibo4MN0
[![雷の形をシミュレーションする方法](https://img.youtube.com/vi/3mVXibo4MN0/0.jpg)](https://www.youtube.com/watch?v=3mVXibo4MN0)

![Demo](./thunder3.mp4)

```
$ gcc main.cpp -o main -Wall
$ ./main [<size>] [-s <seed>] [-e <eta>] [-l] [-d]
```

- `-l` は live mood. デフォルトだと, 1 ステップごとにキー入力待ちになる.
- `-d` は debug mood. Debug mood では, 選択確率もプリントされる.
