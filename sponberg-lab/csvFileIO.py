import csv

with open('file.csv','wb') as f:
    writer = csv.writer(f)
    writer.writerow(['Exponent','Number'])
    for i in range(1,10):
        writer.writerow([str(i ** 2),str(i)])
