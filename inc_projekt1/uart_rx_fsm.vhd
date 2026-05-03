-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Richard Komora (xkomorr00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



entity UART_RX_FSM is
    port(
       CLK : in std_logic;
       RST : in std_logic;
       DIN : in std_logic;
       CNT_8 : in std_logic;
       CNT_16 : in std_logic;
       BIT_8 : in std_logic;    
       CNT_EN : out std_logic;
       RX_EN : out std_logic;
       DOUT_VLD : out std_logic
    );
end entity;




architecture behavioral of UART_RX_FSM is
-- definicia stavov
    type state_type is (IDLE, START_WAIT, DATA_READ, STOP_WAIT, VALID);
    signal stav : state_type;
begin

    --  prechody medzi stavmi
   process (CLK, RST)
   begin
    --  reset ak rst=1 , idle
      if RST = '1' then
         stav <= IDLE;
        -- ak nabezna hrana
      elsif rising_edge(CLK) then 
         case stav is
            -- ked din klesne na 0 tak prichadza startbit -> START_WAIT

            when IDLE =>
               if DIN = '0' then
                  stav <= START_WAIT;
               end if;
            -- cakame 8 taktov, nech sme v strede START BITU
            when START_WAIT =>
               if CNT_8 = '1' then
                  stav <= DATA_READ;
               end if;

            -- docitali sme data
            when DATA_READ =>
               if BIT_8 = '1' then
                  stav <= STOP_WAIT;
               end if;


            when STOP_WAIT =>
               if CNT_16 = '1' then
                  if DIN = '1' then
                      stav <= VALID;
                  end if;
               end if;

            -- po jednom takte naspat do idle
            when VALID =>
               stav <= IDLE;
         end case;
      end if;
   end process;
    
   -- vystupyyy
   process (stav)
   begin
        CNT_EN <= '0';
        RX_EN <= '0';
        DOUT_VLD <= '0';
        -- todo: fix tie zaciatocne hodnoty
      if stav = IDLE then
         CNT_EN <= '0';
         RX_EN <= '0';
         DOUT_VLD <= '0';
      elsif stav = START_WAIT then
         CNT_EN <= '1'; -- pocitadlo na odmeranie 8 taktov
         RX_EN <= '0';
         DOUT_VLD <= '0';
      elsif stav = DATA_READ then
         CNT_EN <= '1';
         RX_EN <= '1'; -- po 16 taktoch ulozime bit z DIN
         DOUT_VLD <= '0';
      elsif stav = STOP_WAIT then
         CNT_EN <= '1';
         RX_EN <= '0';
         DOUT_VLD <= '0';
      elsif stav = VALID then
         CNT_EN <= '0';
         RX_EN <= '0';
         DOUT_VLD <= '1'; -- DOUT ma valid data
      end if;
   end process;

end architecture;