import argparse

# SPIMulator format
# CPIM [DST] [SRC] [OP] [BLOCK SIZE] [WRITE OP]
# CPIM $96 $12 COPY 512 0

def read_trace_lines(filename):
    with open(filename, 'r') as file:
        text = file.read()
        lines = text.split('\n')
        lines = [l for l in lines if len(l) > 0]
        lines = [l[:l.find('//')] if l.find('//') != -1 else l for l in lines]
        lines = [l for l in lines if 'SubByte' not in l]
        return [l for l in lines if l[0] != '#']
    
def tokenize_trace_line(line):
    tokens = line.split(' ')
    tokens = [t for t in tokens if len(t) > 0]
    # Discard CPIM and block size
    return tokens[1:4] + tokens[5:]

def row_number_to_address(row_number):
    # Row number is 12 bits up
    row_number = row_number << 12
    return hex(row_number)

def get_opcode(tokens):
    if tokens[2] in {'STORE'}:
        if tokens[3] != '0':
            return 'T'
        else:
            return 'W'
    elif 'SH' in tokens[2]:
        return 'S'
    elif tokens[2] in {'AND', 'OR', 'XOR', 'COPY', 'PC', 'NOR', 'MULT', 'ADD'}:
        return 'P'
    return '?'

def get_op2(token):
    if token in {'AND', 'OR', 'XOR'}:
        return token
    return 'AND'

def spim_to_rtsim(line):
    tokens = tokenize_trace_line(line)

    rtsim_tokens = ['0', get_opcode(tokens), row_number_to_address(int(tokens[0][1:]))]
    if tokens[2] == 'COPY':
        rtsim_tokens.append('RC')
    elif rtsim_tokens[1] == 'P':
        rtsim_tokens.append(get_op2(tokens[2]))
    elif rtsim_tokens[1] == 'S':
        rtsim_tokens.append(tokens[2][2:])
        rtsim_tokens.append('0')
    elif rtsim_tokens[1] == 'T':
        rtsim_tokens.append(tokens[3])
        rtsim_tokens.append(tokens[1])

    if rtsim_tokens[1] == 'P':
        rtsim_tokens.append(row_number_to_address(int(tokens[1][1:])))
        rtsim_tokens.append('0')
    elif rtsim_tokens[1] == 'W':
        rtsim_tokens.append(tokens[1][2:])

    rtsim_tokens.append('0')
    return ' '.join(rtsim_tokens)

def write_trace(filename, text):
    with open(filename, 'w') as f:
        f.write(text)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', help='SPIMulator trace to convert')

    args = parser.parse_args()

    lines = read_trace_lines(args.filename)

    #for i in range(len(lines)):
    #    print(lines[i])
    #    print(spim_to_rtsim(lines[i]))

    rtsim_lines = [spim_to_rtsim(l) for l in lines]
    rtsim_trace = '\n'.join(rtsim_lines)
    rtsim_filename = args.filename.split('.')[0] + '.nvt'

    print("Writing RTSim trace to " + rtsim_filename)

    write_trace(rtsim_filename, rtsim_trace)