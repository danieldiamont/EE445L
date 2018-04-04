%% Code Cell 1 (Thermistor Plot)

hFig = figure('Resize','off','NumberTitle','off','Toolbar','none',...
'Name','Thermistor Model Interactive Demo');
myhandles = guihandles(hFig); 
R0 = 104500;      % Nominal Resistance at 25C
B0 = 3357.33;      % Nominal Beta Constant at 25C
T0 = 298.15;    % Nominal Temperature 25C in Kelvin
TC = -40:120;   % Temperature range in Celsius
T = TC + 273.15;% Temperature range in Kelvin
% Compute NTC Thermistor Resistance
R = R0.*exp(B0.*((1./T)-(1/T0)));
plot(TC,R,'color','r','linewidth',3);
line([0 0],[0 max(R)],'linestyle','--','linewidth',2);
xlim([-40 120]);
ylim([0 max(R)]);
xlabel('Temperature (^oC)');
ylabel('Resistance (ohm)');
title('Thermistor (NTC) Model');

%% Code Cell 2 (0 to 40 degC focus)

hFig = figure('Resize','off','NumberTitle','off','Toolbar','none',...
'Name','Thermistor Model 0 to 40 degC');
myhandles = guihandles(hFig); 
R0 = 104500;      % Nominal Resistance at 25C
B0 = 3357.33;      % Nominal Beta Constant at 25C
T0 = 298.15;    % Nominal Temperature 25C in Kelvin
TC = 0:40;   % Temperature range in Celsius
T = TC + 273.15;% Temperature range in Kelvin
% Compute NTC Thermistor Resistance
R = R0.*exp(B0.*((1./T)-(1/T0)));
plot(TC,R,'color','r','linewidth',3);
line([0 0],[0 max(R)],'linestyle','--','linewidth',2);
xlim([0 40]);
ylim([0 max(R)]);
xlabel('Temperature (^oC)');
ylabel('Resistance (ohm)');
title('Thermistor (NTC) Model');

%% Input Voltage to Temperature (Celsius) graph
B = 3357.55;
V0 = 0.163;
Vt = 0.004:0.233/4095:0.237;
T0 = 298.15;
R = 500e3;
R0 = 104.5e3;

a = (Vt + V0)/2.5;
num = (R*a)./(1 - a);
denom = R0*exp(-B/T0);

tf = (B./(log(num/denom))) - 273.15 - 10;

figure()
plot(Vt,tf)
title('Temperature (^oC) vs. Voltage (V) Graph')
ylabel('Temperature (^oC)')
xlabel('Voltage Difference Across Bridge (V)')
