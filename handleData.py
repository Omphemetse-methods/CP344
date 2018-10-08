import csv
import matplotlib.pyplot as plt

#Read data from magnetization.csv"
def readData():
	cTime, m = [], [];
	with open("magnetization.csv") as f:
		reader = csv.reader(f);
		next(reader);				# skip header
		for row in reader:
			cTime.append(row[0]);
			m.append(row[1])
	plt.plot(cTime, m);
	plt.show()


if __name__ == "__main__":
	readData()
	print "Hey im python"
