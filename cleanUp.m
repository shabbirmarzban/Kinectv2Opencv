%cleanup

clc
clear all


files = [dir('./*.jpg')];

for i= 1:length(files)
    delete(files(i).name);
end
delete('ticker.txt');