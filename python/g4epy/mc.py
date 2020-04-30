import enum


class McFileTypes (enum.Enum):
    BEAGLE = "beagle"
    HEPMC2 = "hepmc2"
    PYTHIA_BNL = "pythia_bnl"
    PYTHIA_LUND = "pythia_lund"
    UNKNOWN = "unknown"


def detect_mc_type(file_name):
    with open(file_name, 'r') as f:
        line = ""
        try:
            while not line:                 # Just to fake a lot of readlines and hit the end
                line = next(f).replace('\n', '')
        except StopIteration:
            return McFileTypes.UNKNOWN
        if "BEAGLE EVENT FILE" in line:
            return McFileTypes.BEAGLE
        elif "HepMC" in line:
            return McFileTypes.HEPMC2
        elif "PYTHIA EVENT FILE" in line:
            return McFileTypes.PYTHIA_BNL
        elif len(line.split()) == 10:
            return McFileTypes.PYTHIA_LUND
    return McFileTypes.UNKNOWN


def build_file_open_command(file_type: McFileTypes, file_name: str):
    """ Translates McFileTypes to Geant4EIC messenger command

    :param file_name: Name of file to process
    :param file_type: Type of file to open
    :return: array of strings - commands
    """

    cmd_gen_name = ""   # Generator name for G4E command

    if file_type == McFileTypes.BEAGLE:
        cmd_gen_name = "beagle"

    if file_type == McFileTypes.HEPMC2:
        cmd_gen_name = "hepmcAscii"

    if file_type == McFileTypes.PYTHIA_LUND:
        cmd_gen_name = "pythiaAscii"

    if not cmd_gen_name:
        message = "The file type is not supported or was not automatically identified. " \
                  "TODO: Execute '/generator/...' commands to explicitly provide file type.\n" \
                  f"Identified file type: {file_type}, file: '{file_name}'"
        raise NotImplementedError(message)

    return [f"/generator/select {cmd_gen_name}",
            f"/generator/{cmd_gen_name}/open {file_name}"]

