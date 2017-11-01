# -*- coding: utf-8 -*-
# @Author: hectorqiu
# @Date:   2017-09-26 19:57:20

import os
import shutil

from pypinyin import pinyin, Style
from .base import get_filename


def rename_file_to_pinyin_letter(file_path, output_dir=None):

    if not os.path.isfile(file_path):
        return 1, None

    src_filename = os.path.basename(file_path)
    new_filename = get_pinyin_letter_name(src_filename)

    if not output_dir:
        output_dir = os.path.dirname(file_path)

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    dest_path = os.path.join(output_dir, new_filename)

    shutil.copy(file_path, dest_path)

    return 0, dest_path


def get_pinyin_letter_name(filename):

    arr = os.path.splitext(filename)

    name = arr[0]
    ext = arr[1]

    name_pair = name.split(' ')

    arr = pinyin(name_pair[0], style=Style.FIRST_LETTER)

    new_filename = merge_pinyin_arr(arr)

    if len(name_pair) > 1 and name_pair[1] == '二维码':
        new_filename += '_qrcode'

    new_filename += ext

    return new_filename


def merge_pinyin_arr(arr):
    """合并 pypinyin 输出的结果

    :arr: 列表嵌套的列表，如 [['g'], ['z'], ['c'], ['h']]

    :return: 'gzch'
    """

    str_arr = []
    for item in arr:
        str_arr.append(item[0])
    return ''.join(str_arr)
