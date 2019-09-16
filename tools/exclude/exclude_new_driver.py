#!/usr/bin/env python
# _*_ coding:utf-8 _*_

import os
import sys

BOARD_LIST = [] 
paramerror = 0
DRIVER_NAME = 0

if len(sys.argv) >= 3 :

    DRIVER_NAME = sys.argv[1]
    for i in range(len(sys.argv) - 2):
        BOARD_LIST.append(sys.argv[i + 2])
        
else :
    paramerror = 1
    
if paramerror :
    print 'Usage : exclude_new_driver.py [driver_file: soc/xxx/drivers/source/xxx/xxx.c] [board of use this driver]'
	print 'example : exclude_new_driver.py soc/zlg/drivers/source/gpio/am_gpio.c am237_core'
    exit(-1)
    
# 设置环境变量和路径
cur_build_path = os.path.split(os.path.realpath(__file__))[0];
ametal_path = os.path.realpath('cur_build_path\..\..\..')
ametal_board_top_path = os.path.join(ametal_path, 'board')

#操作.cproject,屏蔽指定文件
def exculde_file_in_cproject(cpro_path, file_path):

    driver_path = os.path.join(ametal_path, file_path)
    #首先查看文件是否存在
    if not os.path.exists(driver_path): 
        print 'driver does not exist!'
        return #不存在就退出
        
    #首先查看文件是否存在
    if not os.path.exists(cpro_path): 
        print '.cproject script does not exist!'
        return #不存在就退出

    index = file_path[4:].find('\\')
    if(index < 0) :
        index = file_path[4:].find('/')
    soc_type = file_path[0 : (index + 4)]
    
    with open(cpro_path, "r+") as f:
        lines = f.readlines()
        f.seek(0, 0)
        for line in lines:
            if 'excluding' in line:
                if file_path in line:
                    f.write(line)
                    return
                if (soc_type + '|') in line:
                    f.write(line)
                    return

                str_list = list(line)
                npos = str_list.index('=')
                str_list.insert(npos + 2, file_path + '|')
                new_str = ''.join(str_list)
                f.write(new_str)
                continue
            f.write(line)
    f.close()
    
def exclud_file_or_folder():
    dir_list = os.listdir(ametal_board_top_path)
    for file in dir_list :
        if(file == 'bsp_common') :
            continue
        is_board = 0
        for board_name in BOARD_LIST:
            if(file == board_name) :
                is_board = 1
                break
        
        if(is_board == 1) :
            continue
        else :
            board_path = os.path.join(ametal_board_top_path, file)
            for root, dirs, files in os.walk(board_path, topdown = False):
                for file_name in files :
                    if (file_name == '.cproject'):
                        os.path.join(root, file_name)
                        exculde_file_in_cproject(os.path.join(root, file_name), DRIVER_NAME)
    print 'successful!'
                        
exclud_file_or_folder()

