class Stack:
    def __init__(self):
        self.item = []
        

    def is_empty(self):
        if len(self.item) == 0:
            return True
        return False

    def push(self, item):
        self.item.append(item)

    def peek(self):
        if len(self.item) == 0:
            return 'Error'
        return self.item[-1]

    def pop(self):
        if not self.is_empty():
            return self.item.pop()
        
    def __str__(self):
                self.a=self.item
                self.a.reverse()
                s=''
                for i in self.a[:len(self.a)-1]:
                     s+=str(i)+' '
                s+=str(self.a[-1])
                return s
                
                                        

    def __len__(self):
                return len(self.item)