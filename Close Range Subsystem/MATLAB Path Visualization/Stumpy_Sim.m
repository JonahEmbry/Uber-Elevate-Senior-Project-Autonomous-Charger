% Written by Jonah Embry 12/13/2020
% Sample Code taken from https://pundit.pratt.duke.edu/wiki/MATLAB:Animation
% See link above for simpler example

clear; format short e
figure(1); clf

%% Target Set Up
TarX = -4; %Change to move target horizontally (-10 > x > 10)
TarY = 7; %Change to move target vertically (x > 0) 

hold on
plot(TarX, TarY, 'ro', 'MarkerFaceColor', 'r');
plot([0 TarX], [0 TarY], 'r--');
xlim([-10 10]);
ylim([0 10]);
title('Uber Elevate Close Range System Mk 1.5 Motion Planning');
hold on


%% Arm Set Up

% Arm Data
Length = 8;     %Length of Arm

%% Targeting 
% Initial Position

% Need to determine angle to approach
InY = TarY - 1;
InX = InY *(TarX/TarY);
plot(InX, InY, 'bx');

thetaI = asind(InY/Length);

if(TarX > 0)
    dxI = InX - (Length * cosd(thetaI));
else
    dxI = InX + (Length * cosd(thetaI));
end

TipxT = linspace(0, InX, 100);
TipyT = linspace(8, InY, 100);

BasexT = linspace(0, dxI, 100);
BaseyT = linspace(0, 0, 100);



hold on
Line = plot([BasexT(1) TipxT(1)], [BaseyT(1) TipyT(1)],'b','LineWidth',3);
%Dot = plot(dxT(1), dyT(1),'bo', 'MarkerFaceColor', 'b')
hold off
legend('Target Position', 'Target Path', 'Initital Position', 'Robotic Arm');
M(1) = getframe;

for k=2:length(BasexT)
    set(Line, 'XData', [BasexT(k) TipxT(k)], 'YData', [BaseyT(k) TipyT(k)])
    drawnow
end

% Linear Extrapolate from Initial Position to Final Position

ThetaF = asind(TarY/Length);

%Adjust for Pos/Neg
if(TarX > 0)
    dxF = TarX - (8 * cosd(ThetaF));
else
    dxF = TarX + (8 * cosd(ThetaF));
end


TipxTF = linspace(InX, TarX, 100);
TipyTF = linspace(InY, TarY, 100);

BasexTF = linspace(dxI, dxF, 100);
BaseyTF = linspace(0, 0, 100);

% hold on
% Line = plot([BasexTF(1) TipxTF(1)], [BaseyTF(1) TipyTF(1)], 'k','LineWidth',3);
% %Dot = plot(dxT(1), dyT(1),'bo', 'MarkerFaceColor', 'b')
% hold off
M(1) = getframe;

for k=2:length(BasexTF)
    set(Line, 'XData', [BasexTF(k) TipxTF(k)], 'YData', [BaseyTF(k) TipyTF(k)])
    drawnow
end

% Calculating Angle that RotDOF1 needs to turn

Gamma = atand(TarY/TarX); %Angle that charger needs to be at
Phi = asind(TipyTF./Length);    %Angle that arm (not the tip) is currently at
TipTheta = Gamma - Phi;     %Angle that RotDOF1 needs to turn at each position









