# Partially stolen from https://bitbucket.org/mblum/libgp/src/2537ea7329ef/.ycm_extra_conf.py
import os
import ycm_core

# These are the compilation flags that will be used in case there's no
# compilation database set (by default, one is not set).
# CHANGE THIS LIST OF FLAGS. YES, THIS IS THE DROID YOU HAVE BEEN LOOKING FOR.
flags = [
        '-Wall',
        '-Wextra',
        '-Werror',
        # '-Wc++98-compat',
        '-Wno-long-long',
        '-Wno-variadic-macros',
        '-fexceptions',
        # THIS IS IMPORTANT! Without a "-std=<something>" flag, clang won't know which
        # language to use when compiling headers. So it will guess. Badly. So C++
        # headers will be compiled as C headers. You don't want that so ALWAYS specify
        # a "-std=<something>".
        # For a C project, you would set this to something like 'c99' instead of
        # 'c++11'.
        '-std=c99',
        # ...and the same thing goes for the magic -x option which specifies the
        # language that the files to be compiled are written in. This is mostly
        # relevant for c++ headers.
        # For a C project, you would set this to 'c' instead of 'c++'.
        '-x', 'c',
        # This path will only work on OS X, but extra paths that don't exist are not
        # harmful
        '-isystem', '/usr/include',
        '-isystem', '/usr/local/include',
        '-I', '.',
        '-I', './api/src',
        '-I', './cli/src',
        '-I', './contrib/argp-standalone',
        '-I', './contrib/fuse-include',
        '-I', './contrib/fuse-lib',
        '-I', './contrib/libexecinfo',
        '-I', './contrib/macfuse',
        '-I', './contrib/mount',
        '-I', './contrib/qemu',
        '-I', './contrib/qemu',
        '-I', './contrib/qemu',
        '-I', './contrib/qemu/block',
        '-I', './contrib/qemu/include',
        '-I', './contrib/qemu/include',
        '-I', './contrib/qemu/include',
        '-I', './contrib/qemu/include/block',
        '-I', './contrib/qemu/include/exec',
        '-I', './contrib/qemu/include/fpu',
        '-I', './contrib/qemu/include/migration',
        '-I', './contrib/qemu/include/monitor',
        '-I', './contrib/qemu/include/qapi',
        '-I', './contrib/qemu/include/qapi/qmp',
        '-I', './contrib/qemu/include/qemu',
        '-I', './contrib/qemu/include/sysemu',
        '-I', './contrib/qemu/trace',
        '-I', './contrib/rbtree',
        '-I', './contrib/uuid',
        '-I', './geo-replication/src',
        '-I', './glusterfsd/src',
        '-I', './libglusterfs/src',
        '-I', './rpc/rpc-lib/src',
        '-I', './rpc/rpc-transport/rdma/src',
        '-I', './rpc/rpc-transport/socket/src',
        '-I', './rpc/xdr/src',
        '-I', './tests/basic',
        '-I', './xlators/cluster/afr/src',
        '-I', './xlators/cluster/dht/src',
        '-I', './xlators/cluster/ec/src',
        '-I', './xlators/cluster/ha/src',
        '-I', './xlators/cluster/map/src',
        '-I', './xlators/cluster/stripe/src',
        '-I', './xlators/debug/error-gen/src',
        '-I', './xlators/debug/io-stats/src',
        '-I', './xlators/debug/trace/src',
        '-I', './xlators/encryption/crypt/src',
        '-I', './xlators/encryption/rot-13/src',
        '-I', './xlators/features/barrier/src',
        '-I', './xlators/features/changelog/lib/src',
        '-I', './xlators/features/changelog/src',
        '-I', './xlators/features/compress/src',
        '-I', './xlators/features/filter/src',
        '-I', './xlators/features/gfid-access/src',
        '-I', './xlators/features/glupy/src',
        '-I', './xlators/features/index/src',
        '-I', './xlators/features/locks/src',
        '-I', './xlators/features/mac-compat/src',
        '-I', './xlators/features/marker/src',
        '-I', './xlators/features/path-convertor/src',
        '-I', './xlators/features/qemu-block/src',
        '-I', './xlators/features/quiesce/src',
        '-I', './xlators/features/quota/src',
        '-I', './xlators/features/read-only/src',
        '-I', './xlators/features/snapview-client/src',
        '-I', './xlators/features/snapview-server/src',
        '-I', './xlators/features/trash/src',
        '-I', './xlators/lib/src',
        '-I', './xlators/meta/src',
        '-I', './xlators/mgmt/glusterd/src',
        '-I', './xlators/mount/fuse/src',
        '-I', './xlators/nfs/server/src',
        '-I', './xlators/performance/io-cache/src',
        '-I', './xlators/performance/io-threads/src',
        '-I', './xlators/performance/md-cache/src',
        '-I', './xlators/performance/open-behind/src',
        '-I', './xlators/performance/quick-read/src',
        '-I', './xlators/performance/read-ahead/src',
        '-I', './xlators/performance/readdir-ahead/src',
        '-I', './xlators/performance/write-behind/src',
        '-I', './xlators/playground/template/src',
        '-I', './xlators/protocol/client/src',
        '-I', './xlators/protocol/server/src',
        '-I', './xlators/storage/bd/src',
        '-I', './xlators/storage/posix/src',
        '-I', './xlators/system/posix-acl/src',
		'-I', '/usr/include/glusterfs',
]

# Set this to the absolute path to the folder (NOT the file!) containing the
# compile_commands.json file to use that instead of 'flags'. See here for
# more details: http://clang.llvm.org/docs/JSONCompilationDatabase.html
#
# Most projects will NOT need to set this to anything; you can just change the
# 'flags' list of compilation flags. Notice that YCM itself uses that approach.
compilation_database_folder = ''

if compilation_database_folder:
  database = ycm_core.CompilationDatabase( compilation_database_folder )
else:
  database = None


def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )

def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return list( flags )
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith( '/' ):
        new_flag = os.path.join( working_directory, flag )

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = path_flag + os.path.join( working_directory, path )
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags


def FlagsForFile( filename ):
  if database:
    # Bear in mind that compilation_info.compiler_flags_ does NOT return a
    # python list, but a "list-like" StringVec object
    compilation_info = database.GetCompilationInfoForFile( filename )
    final_flags = MakeRelativePathsInFlagsAbsolute(
      compilation_info.compiler_flags_,
      compilation_info.compiler_working_dir_ )
  else:
    relative_to = DirectoryOfThisScript()
    final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to )

  return {
    'flags': final_flags,
    'do_cache': True
  }

