In order to run noutbooks one has to get data files and run g4e

### Data files

To start with the project, one can use one of the beagle files located here:

[link to a folder on a google drive](https://drive.google.com/drive/folders/1jHLO-w8YZTKUFUIsYmbBjqk58MM41hah?usp=sharing)

Files are large but there is one small sample file with ~30k events (29119)

[link to eC_10x41_GCF_QE_Egaus_small.txt on a google drive](https://drive.google.com/file/d/1C7Mj7rm-Zv21wRTrwzIOhaEHKXZuF7si/view?usp=sharing)


All initial scripts assume that files are downloaded and located in work directory
(directory named **work** is excluded from git to be used as temporary storage)

### Getting g4e files

```bash
python3 generate_test_data.py
```

As the result test_run.root will be generated and is used in these notebooks