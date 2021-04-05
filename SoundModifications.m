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

subplot(4,1,1);
plot(t, Xt);
xlabel('Time'); ylabel('Amplitude');title('Time domain after noramalizating');

pause;

Xf = abs(fft(Xt));
f = 1/5:1/5:fs;
subplot(4,1,2);
plot(f(1:2.5*fs), Xf(1:2.5*fs));
xlabel('Frequency'); ylabel('Magnitude');title('Frequency domain');

pause;

Xf_amp = Xf;
for ff = 1:length(Xf_amp)
    if ff > amp_frq*5
        Xf_amp(ff) = const*Xf_amp(ff);
    end
end 

subplot(4,1,3);
plot(f(1:2.5*fs), Xf_amp(1:2.5*fs));
xlabel('Frequency'); ylabel('Magnitude');title('Frequency domain - Amplified');

Xt_amp = ifft(Xf_amp);
player_amp = audioplayer(Xt_amp, fs);
play(player_amp);
pause;
stop(player);

Xt_shift = Xt;

for tt = 1:length(t)
    Xt_shift(tt) = exp(1i*shift_frq*2*pi*t(tt))*Xt(tt);
end

Xf_shift = abs(fft(Xt_shift));

for ff = 1:length(Xf_shift)
    if ff < shift_frq*5
           Xf_shift(ff) = 0;
    end
end

subplot(4,1,4);
plot(f(1:2.5*fs), Xf_shift(1:2.5*fs));
xlabel('Frequency'); ylabel('Magnitude');title('Frequency domain - Shifted');

player_amp = audioplayer(Xt_shift, fs);
play(player_amp);
pause;
stop(player);
