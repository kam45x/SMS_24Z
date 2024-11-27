delete(instrfindall);
close all;
clear all;

s = serial('COM4');%COM9tojestportutworzonyprzezmikrokontroler
set(s,'BaudRate',115200);
set(s,'StopBits',1);
set(s,'Parity','none');
set(s,'DataBits',8);
set(s,'Timeout',1);
set(s,'InputBufferSize',1000);
set(s,'Terminator',13);
fopen(s);%otwarciekanalukomunikacyjnego

Tp = 0.1; % czas w jakim próbuje regulator
y_all = []; % wektor wyjść obiektu
u_all = []; % wektor wejść sterowanych obiektu
yzad = [];
while length(y_all) <= 250 % zbieramy dużo pomiarów, 249 wartości
    txt = fgetl(s);
    disp(txt);
    eval(char(txt')); % wykonamy co do otrzymujemy
    
    % Wyodrębnienie wartości U i Y
    matches_U = regexp(txt, 'U=([\d.]+)', 'tokens'); % Szuka U=wartość
    matches_Y = regexp(txt, 'Y=([\d.]+)', 'tokens'); % Szuka Y=wartość

    % Konwersja wyników z tokenów do liczb
    u = str2double([matches_U{:}]); % Zamiana tokenów U na liczby
    y = str2double([matches_Y{:}]); % Zamiana tokenów Y na liczby
    
    y_all = [y_all y];
    u_all = [u_all u];
end

figure;
stairs(y_all);

figure;
stairs(u_all);
