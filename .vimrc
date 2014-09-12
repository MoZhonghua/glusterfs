
let g:ycm_global_ycm_extra_conf = "./ycm_extra_conf.py"

let NERDTreeIgnore=['\.o$', '\.ko$', '\.symvers$', '\.order$', '\.mod.c$', '\.swp$', '\.bak$', '\~$', '\.la', '\.am', '\.in', '\.lo', '\.o']

" let NERDTreeSortOrder=['\/$', 'Makefile', 'makefile', '\.c$', '\.cc$', '\.cpp$', '\.h$', '*', '\~$']
" let NERDTreeMinimalUI=1
" let NERDTreeQuitOnOpen=0
" let NERDTreeWinPos = 'right'
" let NERDTreeWinSize = 31
" let g:miniBufExplMinSize=1
" let g:miniBufExplSplitBelow = 1
"
set gdbprg=gdb\ --args\ ./api/examples/glfsxmp\ testvol\ brick-s0
set path+=/usr/local/include/glusterfs
Bundle 'mihaifm/vimpanel'

