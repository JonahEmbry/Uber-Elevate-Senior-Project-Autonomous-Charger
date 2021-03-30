% Written by Jonah Embry 12/22/2020
% Sample Code taken from https://pundit.pratt.duke.edu/wiki/MATLAB:Animation
% See link above for simpler example
% Arm Path Visualization Mk 1.31
% Translational Movement, Base Rotation, Tip Orientation, Interpolation to
% achieve plugging in
% Need to do: Change Initialization (see section for more details)

clear; format short e
figure(1); clf

%% Target Set Up
TarX = 4; %Change to move target horizontally (-10 > x > 10)
TarY = 5; %Change to move target vertically (x > 0) 

hold on
plot(TarX, TarY, 'ro', 'MarkerFaceColor', 'r'); %Plots target point
plot([0 TarX], [0 TarY], 'r--');                %Plots line from target point to origin to show angle of port
xlim([-10 10]);
ylim([0 20]);
title('Uber Elevate Close Range System Mk 1.31 Motion Planning');
hold on


%% Arm Set Up

% Arm Data
Length = 8;     %Length of Arm
CharLeng = 2;   %Charger Length
TotLength = Length + CharLeng;
Steps = 100;       %Number of steps to interpolate through
%% Find Initial Position


% Need to determine angle to approach- This needs to be redone so that
% there is always enough room for the charger head to rotate without
% hitting the port. Right now, the TarY-2 is just a large enough offset
InY = TarY - 2;             %Initial Y position of the end of the arm
InX = InY *(TarX/TarY);     %Initial X position of the end of the arm
plot(InX, InY, 'bx');       %Plots this position on graph

Otheta = atand(TarY/TarX);  %Angle between the port and x-axis going through the origin (all ports are normal to the origin, this need to be changed in the future)
thetaI = asind(InY/Length); %Angle from the arm at the initial position and the x-axis

if(TarX >= 0)               %Changes need to be made if the point features a negative x value
    dxI = InX - (Length * cosd(thetaI));    %Change in x position
    AngleI = linspace(0, thetaI, Steps);    %Array of values for angle between x axis and arm in inital position
    OthetaC = linspace(0, Otheta, Steps);   %Array of values for angle between x axis and charge in final orientation
else
    dxI = InX + (Length * cosd(thetaI));        %Change in x position
    AngleI = linspace(180, thetaI, Steps);      %Array of values for angle between x axis and arm in inital position   
    OthetaC = linspace(0, 180 + Otheta, Steps); %Array of values for angle between x axis and charge in final orientation
end

%% Move along x-axis

BasexT = linspace(-8, dxI, Steps);  %Change in x for the base point of the arm while moving along x-axis
BaseyT = linspace(0, 0, Steps);     %Change in y for the base point of the arm while moving along x-axis
TipyT = linspace(0, InY, Steps);    %Change in y for tip of the arm while moving along x-axis
TipxT = linspace((Steps+8), InX, Steps);    %Change in x for tip of arm while moving along x-axis


hold on
Line = plot([BasexT(1) (BasexT(1)+ Length)], [BaseyT(1) TipyT(1)],'b','LineWidth',3);           %Line for plotting robotic arm
LineT = plot([(BasexT(1)+Length) (BasexT(1)+TotLength)], [BaseyT(1) TipyT(1)],'g','LineWidth',3);   %Line for plotting charger head
hold off
legend('Target Position', 'Target Path', 'Initital Position', 'Robotic Arm', 'Charger');
M(1) = getframe;        %Necessary for animation

% Move along X-axis
for k=2:Steps
    set(Line, 'XData', [BasexT(k) (BasexT(k) + Length)], 'YData', [BaseyT(1) TipyT(1)])             %Moves robotic arm along x-axis
    set(LineT, 'XData', [(BasexT(k) + Length) (BasexT(k) + TotLength)], 'YData', [BaseyT(1) TipyT(1)])  %Moves charger along with arm
    drawnow
end

%% Rotate Robotic Arm
if(TarX >= 0)   %Has to alternate based on position of charger (+/- x)
     for k=2:Steps
         set(Line, 'XData', [BasexT(Steps) ((Length * cosd(AngleI(k)))+ BasexT(Steps))], 'YData', [BaseyT(k) (Length * sind(AngleI(k)))])   %Rotates robotic arm
         set(LineT, 'XData', [((Length * cosd(AngleI(k)))+ BasexT(Steps)) ((TotLength * cosd(AngleI(k)))+ BasexT(Steps))], 'YData', [(Length * sind(AngleI(k))) (Length * sind(AngleI(k)))]) %Moves charger along with arm
         drawnow 
     
     %pause(0.1);
     end
else
     for k=2:Steps
     
        set(Line, 'XData', [BasexT(Steps) (Length * cosd(180-AngleI(k))) + (BasexT(Steps))], 'YData', [BaseyT(k) (Length * sind(180-AngleI(k)))]) %Rotates robotic arm
        set(LineT, 'XData', [(Length * cosd(180-AngleI(k))) + (BasexT(Steps)) (Length * cosd(180-AngleI(k))) + (BasexT(Steps) + CharLeng)], 'YData', [(Length * sind(180-AngleI(k))) (Length * sind(180-AngleI(k)))]) %Moves charger along with arm
        drawnow 

     %pause(0.1);
     end
end

%% Rotate Charger Head
% Rotates Charger Head to Match with Interpolation between Initial and Final

if(TarX >= 0) %Has to alternate based on position of charger (+/- x)
     for k=2:Steps
         ArmTipX = ((Length * cosd(AngleI(Steps)))+ BasexT(Steps)); %X Location of tip of robotic arm
         ArmTipY = (Length * sind(AngleI(Steps)));                  %Y Location of tip of robotic arm
         set(LineT, 'XData', [ArmTipX ((CharLeng * cosd(OthetaC(k)))+ ArmTipX)], 'YData', [ArmTipY ((CharLeng * sind(OthetaC(k))) + ArmTipY)]) %Rotate charger
         drawnow 
     
     %pause(0.1);
     end
else
     for k=2:Steps
        ArmTipX = (Length * cosd(180-AngleI(Steps))) + (BasexT(Steps)); %X Location of tip of robotic arm
        ArmTipY = (Length * sind(180-AngleI(Steps)));                   %Y Location of tip of robotic arm
        set(LineT, 'XData', [ArmTipX ((CharLeng * cosd(OthetaC(k)))+ ArmTipX)], 'YData', [ArmTipY ((CharLeng * sind(OthetaC(k))) + ArmTipY)]) %Rotate charger
        drawnow 

     %pause(0.1);
     end
end



%% Linear Extrapolate from Initial Position to Final Position

ThetaF = asind(TarY/Length); %Angle between x-axis and robotic arm in final position 

%Adjust for Pos/Neg
if(TarX >= 0)
    dxF = TarX - (8 * cosd(ThetaF));
else
    dxF = TarX + (8 * cosd(ThetaF));
end


TipxTF = linspace(InX, TarX, Steps);  %Interpolation between initial and final tip X positions
TipyTF = linspace(InY, TarY, Steps);  %Interpolation between initial and final tip Y positions

BasexTF = linspace(dxI, dxF, Steps);  %Interpolation between initial and final base X positions
BaseyTF = linspace(0, 0, Steps);      %Interpolation between initial and final base Y positions

M(1) = getframe;    %Stuff for animation

for k=2:length(BasexTF)
    set(Line, 'XData', [BasexTF(k) TipxTF(k)], 'YData', [BaseyTF(k) TipyTF(k)]) %Move robotic arm
    set(LineT, 'XData', [TipxTF(k) ((CharLeng * cosd(OthetaC(length(BasexTF))))+ TipxTF(k))], 'YData', [TipyTF(k) ((CharLeng * sind(OthetaC(length(BasexTF)))) + TipyTF(k))] ) %Move charger 
    drawnow
end

% Calculating Angle that RotDOF1 needs to turn (Pretty sure this is wrong,
% ignore for now

% Gamma = atand(TarY/TarX); %Angle that charger needs to be at
% Phi = asind(TipyTF./Length);    %Angle that arm (not the tip) is currently at
% TipTheta = Gamma - Phi;     %Angle that RotDOF1 needs to turn at each position









