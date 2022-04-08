import os
import multiprocessing
from pathlib import Path


def PhonyTargets(
    target,
    action,
    depends,
    env=None,
):
    # Creates a Phony target
    if not env:
        env = DefaultEnvironment()
    t = env.Alias(target, depends, action)
    env.AlwaysBuild(t)


MINGW = 'mingw' in COMMAND_LINE_TARGETS
PROGRAM = "simulated.exe" if MINGW else "simulated"
MAIN = "main"
SIMULATOR = 'simulator'
COMPONENTS = "components"

CFLAGS = [
    "-Wall",
    "-Wextra",
    "-Wno-unused-function",
    "-g",
    "-O0",
    "-DLV_CONF_INCLUDE_SIMPLE",
    "-Wno-unused-parameter",
]
LDLIBS = ["-lmingw32", "-lSDL2main",
          "-lSDL2"] if MINGW else ["-lSDL2"] + ['-lpthread']

CPPPATH = [
    COMPONENTS, f'{SIMULATOR}/port', f'#{MAIN}',
    f"#{MAIN}/config", f"#{SIMULATOR}"
]


def main():
    num_cpu = multiprocessing.cpu_count()
    SetOption('num_jobs', num_cpu)
    print("Running with -j {}".format(GetOption('num_jobs')))

    env_options = {
        "ENV": os.environ,
        "CC": ARGUMENTS.get('cc', 'gcc'),
        "ENV": os.environ,
        "CPPPATH": CPPPATH,
        'CPPDEFINES': [],
        "CCFLAGS": CFLAGS,
        "LIBS": LDLIBS,
    }

    env = Environment(**env_options)
    env.Tool("compilation_db")
    env.Tool("clang")

    sources = Glob(f'{SIMULATOR}/*.c')
    sources += Glob(f'{SIMULATOR}/port/*.c')
    sources += [File(filename) for filename in Path('main/model').rglob('*.c')]
    sources += [File(filename) for filename in Path('main/config').rglob('*.c')]
    # sources += [File(filename) for filename in Path('main/view').rglob('*.c')]
    sources += [File(filename) for filename in Path('main/controller').rglob('*.c')]

    prog = env.Program(PROGRAM, sources)
    PhonyTargets('run', './simulated', prog, env)
    env.Alias('mingw', prog)
    env.CompilationDatabase('build/compile_commands.json')


main()
