import shlex
import subprocess
from datetime import datetime

def run(command, sink):
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
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
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
    sink.done()

    sink.add_line("------------------------------------------")
    sink.add_line(f"RUN DONE. RETVAL: {retval} \n\n")
    if retval != 0:
        sink.add_line(f"ERROR. Retval is not 0. Plese, look at the logs\n")

    return retval, start_time, end_time, lines