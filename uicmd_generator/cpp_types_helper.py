def get_eeprom_value_type(value_type):
    if 'Int8' in value_type:
        return 'uint8_t'
    elif 'Int16' in value_type:
        return 'uint16_t'
    elif 'Int32' in value_type:
        return 'uint32_t'
    elif 'Float' in value_type:
        return 'float'
    else:
        raise ValueError()


def get_eeprom_type_name(value_type):
    if value_type == 'Int8' or value_type == 'UInt8':
        return 'byte'
    elif value_type == 'Int16' or value_type == 'UInt16':
        return 'word'
    elif value_type == 'Int32' or value_type == 'UInt32':
        return 'dword'
    elif value_type == 'Float':
        return 'float'
    elif '[]' in value_type:
        return 'block'
    else:
        raise ValueError


def get_avr_value_type(value_type):
    if value_type == 'UInt8' or value_type == 'UInt8[]':
        return 'uint8_t'
    elif value_type == 'UInt16' or value_type == 'UInt16[]':
        return 'uint16_t'
    elif value_type == 'UInt32' or value_type == 'UInt32[]':
        return 'uint32_t'
    elif value_type == 'Int8' or value_type == 'Int8[]':
        return 'int8_t'
    elif value_type == 'Int16' or value_type == 'Int16[]':
        return 'int16_t'
    elif value_type == 'Int32' or value_type == 'Int32[]':
        return 'int32_t'
    elif value_type == 'Float' or value_type == 'Float[]':
        return 'float'
    else:
        raise ValueError


def get_sscanf_key(value_type):
    if value_type == 'Int8' or value_type == 'UInt8' or value_type == 'Int8[]' or value_type == 'UInt8[]':
        return '%d'
    elif value_type == 'Int16' or value_type == 'UInt16' or value_type == 'Int16[]' or value_type == 'UInt16[]':
        return '%d'
    elif value_type == 'Int32' or value_type == 'UInt32' or value_type == 'Int32[]' or value_type == 'UInt32[]':
        return '%l'
    elif value_type == 'Float' or value_type == 'Float[]':
        return '%f'
    else:
        raise ValueError


def is_array(value_type):
    return "[]" in value_type

def avr_eeprom_value_type(value_type):
    if 'Int8' in value_type:
        return 'uint8_t'
    elif 'Int16' in value_type:
        return 'uint16_t'
    elif 'Int32' in value_type:
        return 'uint32_t'
    elif 'Float' in value_type:
        return 'float'
    else:
        raise ValueError()


def avr_eeprom_read_function(value_type):
    if value_type == 'Int8' or value_type == 'UInt8':
        return 'eeprom_read_byte'
    elif value_type == 'Int16' or value_type == 'UInt16':
        return 'eeprom_read_word'
    elif value_type == 'Int32' or value_type == 'UInt32':
        return 'eeprom_read_dword'
    elif value_type == 'Float':
        return 'eeprom_read_float'
    elif '[]' in value_type:
        return 'eeprom_read_block'
    else:
        raise ValueError()

def avr_eeprom_update_function(value_type):
    if value_type == 'Int8' or value_type == 'UInt8':
        return 'eeprom_update_byte'
    elif value_type == 'Int16' or value_type == 'UInt16':
        return 'eeprom_update_word'
    elif value_type == 'Int32' or value_type == 'UInt32':
        return 'eeprom_update_dword'
    elif value_type == 'Float':
        return 'eeprom_update_float'
    elif '[]' in value_type:
        return 'eeprom_update_block'
    else:
        raise ValueError()


def avr_value_type(value_type):
    if value_type == 'UInt8' or value_type == 'UInt8[]':
        return 'uint8_t'
    elif value_type == 'UInt16' or value_type == 'UInt16[]':
        return 'uint16_t'
    elif value_type == 'UInt32' or value_type == 'UInt32[]':
        return 'uint32_t'
    elif value_type == 'Int8' or value_type == 'Int8[]':
        return 'int8_t'
    elif value_type == 'Int16' or value_type == 'Int16[]':
        return 'int16_t'
    elif value_type == 'Int32' or value_type == 'Int32[]':
        return 'int32_t'
    elif value_type == 'Float' or value_type == 'Float[]':
        return 'float'
    else:
        raise ValueError()


def avr_sscanf_key(value_type):
    if value_type == 'Int8' or value_type == 'UInt8' or value_type == 'Int8[]' or value_type == 'UInt8[]':
        return '%d'
    elif value_type == 'Int16' or value_type == 'UInt16' or value_type == 'Int16[]' or value_type == 'UInt16[]':
        return '%d'
    elif value_type == 'Int32' or value_type == 'UInt32' or value_type == 'Int32[]' or value_type == 'UInt32[]':
        return '%l'
    elif value_type == 'Float' or value_type == 'Float[]':
        return '%f'
    else:
        raise ValueError()


def is_array(value_type):
    return "[]" in value_type


avr_all_filters = {
    'to_sscanf_key': avr_sscanf_key,
    'to_avr_type': avr_value_type,
    'to_eeprom_read_function': avr_eeprom_read_function,
    'to_eeprom_update_function': avr_eeprom_update_function,
    'to_eeprom_value_type': avr_eeprom_value_type
}
