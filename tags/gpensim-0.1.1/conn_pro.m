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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% File: conn_pro.m: Definition of the connections between the modules
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [PN_name, set_of_places, set_of_trans, set_of_arcs] ...
    = conn_pro(global_info)

PN_name = 'Connection Profile';
set_of_places = [];
set_of_trans = [];
set_of_arcs = { ...
    'pDIST','tTX',1, ...    % Server -> Network (Transmit)
    'tRX','pCOL',1, ...     % Network -> Server (Receive)
};

%    'pTX','tGET',1, ...     % Network -> Client (HTTP GET)
%    'pPUT','pRX',1, ...     % Client -> Network (HTTP PUT)
