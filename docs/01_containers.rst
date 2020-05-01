Containers
==========


Docker quick start
------------------


.. code:: bash

    docker run --rm -it -p 8888:8888 electronioncollider/escalate:latest


The command will start the jupyter lab server with example files. After running this command,
open your browser

``--rm`` flag Docker **automatically cleans up the container** and removes the file system **when
the container exits**. By default (without --rm flag) a container’s file system persists
even after the container exits. This makes debugging easier and you retain all your data.
[More on the docker docs](https://docs.docker.com/engine/reference/run/#clean-up---rm).
We use --rm flag for the sake of the tutorial repeatability. **If you WORK with the container, you probably
want to run without --rm**.


``-it`` flag enables interactive session. Without this flag ctrl+c will not work on Mac
machines. In general ``-it`` is used to run e.g. bash session (see below)


More on Jupyter

- `Jupyter community guides <https://jupyter.readthedocs.io/en/latest/community/content-community.html>`_
- `Python Data Science Handbook <https://github.com/jakevdp/PythonDataScienceHandbook>`_


Run without jupyter lab
-----------------------

You can start the docker without Jupyter Lab running:


.. code:: bash

   docker run --rm -it -p 8888:8888 electronioncollider/escalate:latest bash


Later you can still run jupyter lab by

.. code:: bash

    jlab    # It is Jupyter Lab here, not Jefferson


Software installed
------------------

What software is installed and where is it?

In Jupyter lab terminal


Changing and saving the contents
--------------------------------

The files that you see in Jupyter Lab are stored in Git repository:

https://gitlab.com/eic/escalate/workspace

So when you do the change, that is important to save, you could make
you branch or fork the repo. This


### Troubleshooting
If docker gives an error like this:
> Error starting userland proxy: listen tcp 0.0.0.0:8888: bind: address already in use.

It usually means, that the port 8888 is used by another application.
To fix that try to change `-p 8888:8888` flag to `-p <something>:8888`
e.g. `-p 9999:8888`. Put the same port in your browser:
```
127.0.0.1:9999/lab
```