%make_video_original

clc
clear all
close all

filenames = dir('./*.jpg');
filenames = extractfield(filenames,'name');
timestamp = zeros(1,length(filenames));
myvideo = VideoWriter('BerkerLaughter1.avi','Motion JPEG AVI');
myvideo.FrameRate = 30;
open(myvideo);

for i = 1: length(filenames)
    temp = textscan(filenames{i},'%s');
    timestamp(i) = str2num(temp{1}{1}(1:end-4));
end
 A = sort(timestamp);
 B = load ('ticker.txt');
 
 frame  =  imread([num2str(A(1)),'.jpg']);
 j=1;
for i = 1: length(B)
    if B(i) ~= 0 && j <= length(A)
        frame  =imread([num2str(A(j)),'.jpg']);   
        j=j+1; 
    end
    writeVideo(myvideo,frame);
    disp(['Frames Done:',num2str(i)])
end

close(myvideo);