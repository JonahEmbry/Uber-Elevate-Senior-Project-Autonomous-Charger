% Written by Jonah Embry 12/13/2020
% Sample Code taken from https://pundit.pratt.duke.edu/wiki/MATLAB:Animation
% See link above for simpler example

clear; format short e
figure(1); clf

%% Target Set Up
TarX = 0; %Change to move target horizontally (-10 > x > 10)
TarY = 2; %Change to move target vertically (x > 0) 

hold on
plot(TarX, TarY, 'ro', 'MarkerFaceColor', 'r');
plot([0 TarX], [0 TarY], 'r--');
xlim([-10 10]);
ylim([0 20]);
title('Uber Elevate Close Range System Mk 1.5 Motion Planning');
hold on


%% Arm Set Up

% Arm Data
Length = 8;     %Length of Arm
CharLeng = 3;   %Charger Length

%% Targeting 
% Initial Position

% Need to determine angle to approach
InY = TarY - 1;
InX = InY *(TarX/TarY);
plot(InX, InY, 'bx');

thetaI = asind(InY/Length);

if(TarX >= 0)
    dxI = InX - (Length * cosd(thetaI));
    AngleI = linspace(0, thetaI, 200);
else
    dxI = InX + (Length * cosd(thetaI));
    AngleI = linspace(180, thetaI, 200);
end


BasexT = linspace(-8, dxI, 200);
BaseyT = linspace(0, 0, 200);
TipyT = linspace(0, InY, 200);
TipxT = linspace((length(BasexT)+8), InX, 200);


hold on
Line = plot([BasexT(1) (BasexT(1)+8)], [BaseyT(1) TipyT(1)],'b','LineWidth',3);
%Dot = plot(dxT(1), dyT(1),'bo', 'MarkerFaceColor', 'b')
hold off
legend('Target Position', 'Target Path', 'Initital Position', 'Robotic Arm');
M(1) = getframe;

% Move along X-axis
for k=2:length(BasexT)
    set(Line, 'XData', [BasexT(k) (BasexT(k) + 8)], 'YData', [BaseyT(1) TipyT(1)])
    drawnow
end

% % Rotate
if(TarX >= 0)
     for k=2:length(BasexT)
         set(Line, 'XData', [BasexT(length(BasexT)) ((Length * cosd(AngleI(k)))+ BasexT(length(BasexT)))], 'YData', [BaseyT(k) (Length * sind(AngleI(k)))])
        drawnow 
     
     %pause(0.1);
     end
else
     for k=2:length(BasexT)
     
        set(Line, 'XData', [BasexT(length(BasexT)) (Length * cosd(180-AngleI(k))) + (BasexT(length(BasexT)))], 'YData', [BaseyT(k) (Length * sind(180-AngleI(k)))])
        drawnow 

     %pause(0.1);
     end
end




% Linear Extrapolate from Initial Position to Final Position

ThetaF = asind(TarY/Length);

%Adjust for Pos/Neg
if(TarX >= 0)
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









