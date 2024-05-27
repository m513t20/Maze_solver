def check():
    colors=set()
    a=True
    with open('debug.txt') as source:
        for i in range(691):
            line=source.readline()
            var=line.split(' ')

            for cur in var:
                colors.add(cur)
    print(len(colors),colors)
check()
input()
