bats<-read.csv(file.choose(),sep=",",header=TRUE);

hist(bats$Count, main="Histogram Bat Count", xlab="Count");

boxplot(bats$Count, main="Box Plot Bat Count", xlab="Count");

qqnorm(bats$Count);

qqline(bats$Count);

