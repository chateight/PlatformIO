import pygame
from pygame.locals import *
import sys, random
import serial
from make_maze import MazeByMakingWall

# maze game using pygame library
# source from https://news.mynavi.jp/techplus/article/zeropython-90/
# maze making algorithm was update

# 変数の初期化
maze_w = 31 # 迷路の列数    (need to be larger than 5 and odd number)
maze_h = 31 # 迷路の行数    (need to be larger than 5 and odd number)
#maze = [] # 迷路データ
tile_w = 16
px = 1 # プレイヤーの座標
py = 1

# 色を定義
black = (0, 0, 0)
red = (255, 0, 0)
white = (255,255,255)
brown = (115, 66, 41)
orange = (233,168, 38)
maze_color = [white, brown, orange]

# メッセージダイアログを表示するのに必要な宣言
from tkinter import *
from tkinter import messagebox
Tk().wm_withdraw()

# プレイヤーの移動を確認 
def check_key(key, maze):
    global px, py
    old_x, old_y = px, py
    if key == "l":
        px -= 1
    elif key == "r":
        px += 1
    elif key == "u":
        py -= 1
    elif key == "d":
        py += 1
    if maze[py][px] == 2: # ゴール?
        messagebox.showinfo("Goal", "I reached here")
    if maze[py][px] != 0: # check if the player collides with the wall
        px, py = old_x, old_y

def main():
    # initialize bt serial, serial port depends on the running enviroment
    m5data = serial.Serial('/dev/tty.ESP32test',115200, timeout=5)
    maze_wall = MazeByMakingWall(maze_w, maze_h)    # maze generate class instance
    maze = maze_wall.make_maze_wall()               # maze generate method call          
    maze[maze_h-2][maze_w-2] = 2                    # marking the goal position

    # ゲームの初期化処理
    global px, py
    pygame.init()
    pygame.display.set_caption("Maze")
    screen = pygame.display.set_mode((tile_w*maze_w,tile_w*maze_h))

    # ゲームのメインループ
    while True:
        # 迷路を描画する
        screen.fill(black)
        for y in range(0, maze_h):
            for x in range(0, maze_w):
                v = maze[y][x]
                xx = tile_w * x
                yy = tile_w * y
                pygame.draw.rect(screen,
                        maze_color[v],
                        (xx,yy,xx+tile_w,yy+tile_w))
        # プレイヤーを円で描画する
        t2 = tile_w / 2
        pygame.draw.circle(screen, red,
                (px * tile_w + t2, py * tile_w + t2), t2)
        pygame.display.update()

        # read from bt and eliminate '\r\n'        
        line = m5data.readline().decode()[:-2:]
        
        if line != "" and line != "t":
            check_key(line, maze)
            print(line)
        if line == "t":         # if the button "B" was pushed, terminate the script
            m5data.close()
            pygame.quit()
            sys.exit()
        
if __name__ == '__main__':
    main()
