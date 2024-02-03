import time

class Utils:
    def __init__(self, Pros_name, Len = 15) -> None:
        self.name = Pros_name
        self.Len = Len

    def INFO (self, *info):
        l = len(self.name)
        s = ""
        for i in range (self.Len - l):
            s += '-'
        s += '>'
        print ('[', time.strftime("%Y-%m-%D %H:%M:%S.", time.localtime()), str(int(1000000*(time.time() - int(time.time())))), ']', self.name, s,  *info)

