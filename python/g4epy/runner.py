import shlex
import subprocess
from datetime import datetime

import sys
import asyncio

if sys.platform == "win32":
    asyncio.set_event_loop_policy(asyncio.WindowsProactorEventLoopPolicy())


async def _read_stream(stream, cb):
    while True:
        line = await stream.readline()
        if line:
            cb(line)
        else:
            break


async def stream_subprocess(cmd, stdout_cb, stderr_cb):
    try:
        process = await asyncio.create_subprocess_exec(
            *cmd, stdout=asyncio.subprocess.PIPE, stderr=asyncio.subprocess.PIPE
        )

        await asyncio.wait(
            [
                _read_stream(process.stdout, stdout_cb),
                _read_stream(process.stderr, stderr_cb),
            ]
        )
        rc = await process.wait()
        return process.pid, rc
    except OSError as e:
        # the program will hang if we let any exception propagate
        return e


def do_execute(*aws):
    """ run the given coroutines in an asyncio loop
    returns a list containing the values returned from each coroutine.
    """
    loop = asyncio.get_event_loop()
    rc = loop.run_until_complete(asyncio.gather(*aws))
    loop.close()
    return rc

def sink_printer(sink):
    def pr(*args, **kw):
        sink.add_line(args[0])

    return pr

def console_printer(label):
    def pr(*args, **kw):
        print(label, *args, **kw)

    return pr


def name_it(start=0, template="s{}"):
    """a simple generator for task names
    """
    while True:
        yield template.format(start)
        start += 1


def runners(cmds):
    """
    cmds is a list of commands to excecute as subprocesses
    each item is a list appropriate for use by subprocess.call
    """
    next_name = name_it().__next__
    for cmd in cmds:
        name = next_name()
        out = console_printer(f"{name}.stdout")
        err = console_printer(f"{name}.stderr")
        yield stream_subprocess(cmd, out, err)


if __name__ == "__main__":
    cmd = [
        "/home/romanov/eic/g4e-dev/cmake-build-debug/g4e",
        "hepmc_full.mac",
        "-o",
        "success"
    ]


    #print(execute(*runners(cmds)))
    do_execute(stream_subprocess(cmd, console_printer("stdout"), console_printer("stderr")))


def run(command, sink, cwd=None, shell=False):
    """Wrapper around subprocess.Popen that returns:

    :return retval, start_time, end_time, lines
    """
    if isinstance(command, str):
        command = shlex.split(command)

    # Pretty header for the command
    sink.add_line('=' * 20)
    sink.add_line("RUN: " + " ".join(command))
    sink.add_line('=' * 20)

    # Record the start time
    start_time = datetime.now()
    lines = []

    # stderr is redirected to STDOUT because otherwise it needs special handling
    # we don't need it and we don't care as C++ warnings generate too much stderr
    # which makes it pretty much like stdout
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=cwd, shell=shell)
    while True:
        line = process.stdout.readline().decode('latin-1').replace('\r', '\n')

        if process.poll() is not None and line == '':
            break
        if line:
            if line.endswith('\n'):
                line = line[:-1]
            sink.add_line(line)
            lines.append(line)

    # Get return value and finishing time
    retval = process.poll()
    end_time = datetime.now()
    sink.add_line("------------------------------------------")
    sink.add_line(f"RUN DONE. RETVAL: {retval} \n\n")
    if retval != 0:
        sink.add_line(f"ERROR. Retval is not 0. Plese, look at the logs\n")

    sink.done()
    return retval, start_time, end_time, lines