# qm_xlogger

import os

g_skip_files=['comm/tinyxml2.cpp', 'comm/tinyxml2.h', 'comm/thread/spinlock.h', 'comm/thread/atomic_oper.h', 'comm/MarcoToolkit.h', 'comm/adler32.h', 'comm/adler32.c', 'comm/md5.h', 'comm/md5.c', 'comm/strutil.cpp']
g_skip_paths=['comm/boost', 'comm/corepattern/test', 'comm/test_case', 'kvcomm/src/protobuf', 'comm/win32', 'comm/wp8', 'comm/windows', 'comm/xlogger', 'netchecker/src/tools', 'comm/objc', 'comm/jni', 'comm/crypt', 'network/src/MMNetProfiler']
g_old_files=[]
g_scan_paths=['log/src', 'comm/', 'network/src', 'streamcdn/src', 'netchecker/src']

for i in range(len(g_skip_files)) :
    g_skip_files[i] = os.path.normpath('.' + os.path.sep + g_skip_files[i])
    print( g_skip_files[i])
    
for i in range(len(g_skip_paths)) :
    g_skip_paths[i] = os.path.normpath('.' + os.path.sep + g_skip_paths[i])
    
for i in range(len(g_old_files)) :
    g_old_files[i] = os.path.normpath('.' + os.path.sep + g_old_files[i])
    
for i in range(len(g_scan_paths)) :
    g_scan_paths[i] = os.path.normpath('.' + os.path.sep + g_scan_paths[i])
    
def is_skip_file(file_path):
    if file_path in g_skip_files:
        return True
    else :
        return False

def is_skip_path(file_path):
    if file_path in g_skip_paths:
        return True
    else :
        return False
    
def is_old_file(file_path):
    if file_path in g_old_files:
        return True
    else :
        return False
        
def get_filters():
    return '-whitespace/braces, -whitespace/line_length, -whitespace/end_of_line, -build/header_guard, -build/include, -runtime/references, -readability/casting'

def get_scan_paths():
    return g_scan_paths
