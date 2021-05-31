close all;
clear all;

shift_frq = 10000;      % shift to this frequency
amp_frq = 2000;         % amplify after this frequency
const = 15;             % amplyfing coefficient 

AudioFile = 'sound.wav';
[inputSound, fs] = audioread(AudioFile);

player = audioplayer(inputSound, fs);
play(player);
pause;
stop(player);

N = fs*5;
t = (1:N)/fs;
Xt = inputSound(1:N,1);
Xt = Xt/max(abs(Xt));

%sound(Xt, fs);
%pause;
%stop(player);

subplot(5,1,1);
plot(t, Xt);
xlabel('Time'); ylabel('Amplitude');title('Time domain after noramalizating');

pause;

Xf = abs(fft(Xt));
f = 1/5:1/5:fs;
subplot(5,1,2);
plot(f, Xf);
xlabel('Frequency'); ylabel('Magnitude');title('Frequency domain');

pause;

Len_Xf = length(Xf);                %Length of total freq. spectrum
Freq_Domain = fs*(0:(Len_Xf/2))/Len_Xf;

subplot(5,1,3);
plot(Freq_Domain,freq_onesided(Xf,Len_Xf));
xlabel('Frequency'); ylabel('Magnitude');title('Frequency domain - One-Sided');

Xf_amp = freq_amp(Xf,1000,6400,0.002);      %Amplifying frequency components between 1000-6400 by 0.002

subplot(5,1,4);
plot(Freq_Domain, freq_onesided(Xf_amp,Len_Xf));
xlabel('Frequency'); ylabel('Magnitude');title('Frequency domain - Amplified (One-Sided)');

Xt_amp = ifft(Xf_amp);
player_amp = audioplayer(Xt_amp, fs);
play(player_amp);
pause;
stop(player);

Xt_shift = Xt;

for tt = 1:length(t)
    Xt_shift(tt) = exp(0i*shift_frq*2*pi*t(tt))*Xt(tt);
end

Xf_shift = abs(fft(Xt_shift));

for ff = 1:length(Xf_shift)
    if ff < shift_frq*5
           Xf_shift(ff) = 0;
    end
end

subplot(5,1,5);
plot(f(1:2.5*fs), Xf_shift(1:2.5*fs));
xlabel('Frequency'); ylabel('Magnitude');title('Frequency domain - Shifted');

player_amp = audioplayer(Xt_shift, fs);
play(player_amp);
pause;
stop(player);

scalingfactor=0.8;
fsscaled=scalingfactor*fs;
tn=linspace(0,length(Xt)/fsscaled,length(Xt));
subplot(6,1,6);
plot(tn,Xt);
xlabel('Time'); ylabel('Magnitude');title('Time domain - Scaled');

player = audioplayer(inputSound, fsscaled);
play(player);
pause;
stop(player);