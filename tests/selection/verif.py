
content = open('logfile').read().split('\n')

digits = [str(i) for i in range(10)]
i = 0
for line in content:
    failed = False
    for d in digits:
        if len(line.split(d)) != 2 and line != "========================" and len(line.split('Test')) == 1 and len(line)>10:
            failed = True
    if failed:
        print("Wrong line encoutered !")
        print(str(i)+" => "+line)
    i += 1
