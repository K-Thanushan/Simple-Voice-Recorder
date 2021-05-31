function echo_wave(AudioFile,depth)
%This function will add echo to the audio
[inputSound, fs] = audioread(AudioFile);

x= length(inputSound);

for fact = 1:depth
    diminished = inputSound*(fact/10);
    for itm = 1:x-3000
        inputSound(itm+3000*fact) = inputSound(itm+3000*fact)+ diminished(itm);
    end
end

player = audioplayer(inputSound, fs);
play(player);
pause;
stop(player);

audiowrite('sound_echo.wav',inputSound,fs);
end