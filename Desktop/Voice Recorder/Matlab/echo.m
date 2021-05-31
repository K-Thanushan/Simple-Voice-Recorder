close all;
clear all;

AudioFile = 'sound.wav';
[inputSound, fs] = audioread(AudioFile);

x= length(inputSound);

for fact = 1:9
    diminished = inputSound*(fact/10);
    for itm = 1:x-3000
        inputSound(itm+3000*fact) = inputSound(itm+3000*fact)+ diminished(itm);
    end
end

player = audioplayer(inputSound, fs);
play(player);
pause;
stop(player);

audiowrite('sound_echo1.wav',inputSound,fs);