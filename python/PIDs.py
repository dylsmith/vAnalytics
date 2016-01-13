import pickle

def LoadVI(VINLength):
    PIDs = []
    with open('C:\\Users\\Dylan\\PycharmProjects\\Rhymalizer\\input.txt') as inf:
        VIN = inf.read(VINLength)
        print('VIN:', VIN)
        byte = inf.read(1)
        return list(map(ord,inf.read()))

#print(LoadVI(0))

def loadPIDs(PIDFile):
    return pickle.load(open("PIDFile", "rb"))

def dumpPIDs(PIDFile):
    PIDs = {}
    with open('input.txt') as inf:
        for line in inf:
            line = line.split(' ')
            line[0] = int(line[0], 16)
            line[1] = line[1].strip()
            PIDs[line[0]] = line[1]
    pickle.dump(PIDs, open("PIDs", "wb"))