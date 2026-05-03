-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Richard Komora (xkomorr00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0);
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is
    signal cnt_8  : std_logic;
    signal cnt_16: std_logic;
    signal bit_8  : std_logic;
    signal cnt_en : std_logic;
    signal rx_en  : std_logic;
    --  pocitadla
    signal clks   : std_logic_vector(4 downto 0); 
    signal bites   : std_logic_vector(3 downto 0); 
    signal accepted_bites : std_logic_vector(7 downto 0); 

begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK      => CLK,
        RST      => RST,
        DIN      => DIN,
        CNT_8    => cnt_8,
        CNT_16   => cnt_16,
        BIT_8    => bit_8,
        CNT_EN   => cnt_en,
        RX_EN    => rx_en,
        DOUT_VLD => DOUT_VLD
    );

    process (CLK, RST)
    begin
        if RST = '1' then
            clks   <= "00000";
            bites   <= "0000";
            accepted_bites <= "00000000";
        elsif rising_edge(CLK) then 
            
            if cnt_en = '1' then
                clks <= clks + 1;
            else
                -- ak neni enable nulujeme
                clks <= "00000";
                bites <= "0000";
            end if;

            -- po 8 taktoch reset 
            if cnt_en = '1' and rx_en = '0' and clks = "00111" and bites = "0000" then
                clks <= "00000";
            end if;

            -- ked cita tak po 16 taktoch reset
            if cnt_en = '1' and rx_en = '1' and clks = "01111" then
                clks <= "00000";
                
                -- stred bitu si ulozime
                accepted_bites <= DIN & accepted_bites(7 downto 1);
                
                bites <= bites + 1;
            end if;

        end if;
    end process;

    cnt_8  <= '1' when clks = "00111" else '0';
    cnt_16 <= '1' when clks = "01111" else '0';
    bit_8  <= '1' when bites = "1000" else '0';

    DOUT <= accepted_bites;

end architecture;
