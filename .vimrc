let g:ycm_global_ycm_extra_conf = "./ycm_extra_conf.py"

let NERDTreeIgnore=['\.o$', '\.ko$', '\.symvers$', '\.order$', '\.mod.c$', '\.swp$', '\.bak$', '\~$', '\.la', '\.in', '\.lo', '\.o']

let NERDTreeSortOrder=['\/$', 'Makefile', 'makefile', '*', '\~$']
let NERDTreeMinimalUI=1
let NERDTreeQuitOnOpen=0
let NERDTreeWinPos = 'right'
let NERDTreeWinSize = 31
autocmd VimEnter * NERDTree
wincmd w
" move cursor from NERDTree to file
autocmd VimEnter * wincmd w 

" set gdbprg=gdb\ --args\ /home/gfs-admin/glusterfs/glusterfsd/src/glusterfs\ -s\ x2\ --volfile-id\ gluster/nfs
set gdbprg=gdb\ --args\ /usr/local/sbin/glusterd
set gdbprg=gdb\ --args\ /usr/local/sbin/gluster\ volume\ info
set gdbprg=gdb\ --args\ /home/gfs-admin/glusterfs/api/examples/open_files_test\ -w\ 1\ -s\ 4096\ -n\ 1\ \-v\ x2\ afrvol
set path+=/usr/include/glusterfs,/include/glusterfs
let g:ctrlp_match_window = 'top'

set makeprg=make\ -C\ /home/gfs-admin/glusterfs/api/examples/\ open_files_test

" autocmd VimEnter * edit ./api/examples/glfsxmp.c
" autocmd VimEnter * split

" donot consider -> . as word sepatator, so we can use <cword> to select 
" a->b, or a.b
set iskeyword+=45,46,62
set tabstop=8


