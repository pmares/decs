%%% DECS GPenSIM - Generic Distributed Exact Cover Solver (DECS) Petri net model
%%% Copyright (C) 2007  Jan Magne Tjensvold
%%% 
%%% This program is free software; you can redistribute it and/or modify it
%%% under the terms of the GNU General Public License version 2 as published
%%% by the Free Software Foundation.
%%% 
%%% This program is distributed in the hope that it will be useful, but
%%% WITHOUT ANY WARRANTY; without even the implied warranty of
%%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
%%% General Public License for more details.
%%% 
%%% You should have received a copy of the GNU General Public License along
%%% with this program; if not, write to the Free Software Foundation, Inc.,
%%% 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
%%% 
%%%
%%%%%%% SOURCE FILE %%%%%%%
%%%%
%%% THE MSF (main simulation file) for the Generic Distributed Exact Cover
%%% Solver (DECS) model.
%%% 
%%% DECS GPenSIM
%%% Version 0.1
%%%%
%%
%% The petri net definition files (PDFs) are programmed separately:
%% PDF for module "Server" defined in "server_def.m"
%% PDF for module "Network" defined in "network_def.m"
%% PDF for module "Client" defined in "client_def.m"
%% Topology of connections between modules defined in "conn_pro.m"

clear, clc;

% Global parameters.
global_info.clients = 12;  % Number of distributed computing clients.
global_info.ptx_max = 4;   % Max. tokens in the network throttling place pTX.
global_info.jobs = 2;      % Max. jobs assigned to one client at the same time.
global_info.pieces = 72;   

global_info.piece_size = 100;   
global_info.result_size = 8000;   
global_info.bw_upstream = 400;   
global_info.bw_downstream = 2000;   

% Maximum number of steps that GPenSIM will process.
global_info.MAX_LOOP = 1000;
global_info.MAX_LOG_SIZE = global_info.MAX_LOOP;

%% Connect the subsystems together
png = petrinetgraph({ ...
    'server_def', ...
    'network_def', ...
    'client_def', ...
    'conn_pro', ...
    }, global_info);

%% Define initial dynamics
dyn.initial_markings = {'pREQ',1};

dyn.firing_times = { ...
    'tTR',0.160, ...
    'tRTR',39, ...
    'tDIV',0.12, ...
    'tMRG',7.2, ...
    'tTX',1/(global_info.piece_size/global_info.bw_upstream), ...
    'tRX',1/(global_info.result_size/global_info.bw_downstream), ...
    };

% 'tTX',0.25, ...
% 'tRX',4, ..


for i=1:global_info.clients,
    dyn.initial_markings = [dyn.initial_markings, ['pFREE', num2str(i)], global_info.jobs];
    dyn.firing_times = [ ...
        dyn.firing_times, ...
        ['tGET', num2str(i)], 'normrnd(0.2,0.02)', ...
        ['tPUT', num2str(i)], 'normrnd(0.2,0.02)', ...
        ['tCOMP', num2str(i)], 'unifrnd(480,720)', ...
        ];
end;

%% Run the simulation
RES = gpensim(png, dyn, global_info);

%% Print the simulation results
printsys(png, RES);


%hold all;
%X = [5:5:25];
%plot(X, Completion_Time);
%plot(X, Completion_Time./X);
%plot(X, (Completion_Time-8.4));
%legend();
%hold off;
plotp(png, RES, {'pDIST', 'pCOL'});
%plotp(png, RES, {'pDIST', 'pCOL', 'pTX', 'pRX'});
%plotp(png, RES, {'pREQ', 'pDIV', 'pDIST', 'pCOL', 'pRTR', 'pRES'});
