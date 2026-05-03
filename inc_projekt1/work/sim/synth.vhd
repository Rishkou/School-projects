library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
entity UART_RX is
  port (
    CLK: in std_logic;
    RST: in std_logic;
    DIN: in std_logic;
    DOUT: out std_logic_vector (7 downto 0);
    DOUT_VLD: out std_logic
  );
end entity;
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart_rx_fsm is
  port (
    clk : in std_logic;
    rst : in std_logic;
    din : in std_logic;
    cnt_8 : in std_logic;
    cnt_16 : in std_logic;
    bit_8 : in std_logic;
    cnt_en : out std_logic;
    rx_en : out std_logic;
    dout_vld : out std_logic);
end entity uart_rx_fsm;

architecture rtl of uart_rx_fsm is
  signal stav : std_logic_vector (2 downto 0);
  signal n71_o : std_logic;
  signal n73_o : std_logic_vector (2 downto 0);
  signal n75_o : std_logic;
  signal n77_o : std_logic_vector (2 downto 0);
  signal n79_o : std_logic;
  signal n81_o : std_logic_vector (2 downto 0);
  signal n83_o : std_logic;
  signal n85_o : std_logic_vector (2 downto 0);
  signal n86_o : std_logic;
  signal n88_o : std_logic;
  signal n90_o : std_logic;
  signal n91_o : std_logic_vector (4 downto 0);
  signal n94_o : std_logic_vector (2 downto 0);
  signal n102_o : std_logic;
  signal n104_o : std_logic;
  signal n106_o : std_logic;
  signal n108_o : std_logic;
  signal n110_o : std_logic;
  signal n113_o : std_logic;
  signal n116_o : std_logic;
  signal n118_o : std_logic;
  signal n120_o : std_logic;
  signal n123_o : std_logic;
  signal n125_o : std_logic;
  signal n127_o : std_logic;
  signal n129_o : std_logic;
  signal n131_o : std_logic;
  signal n133_o : std_logic;
  signal n136_o : std_logic;
  signal n139_o : std_logic;
  signal n142_q : std_logic_vector (2 downto 0);
begin
  cnt_en <= n133_o;
  rx_en <= n136_o;
  dout_vld <= n139_o;
  -- work/uart_rx_fsm.vhd:30:12
  stav <= n142_q; -- (signal)
  -- work/uart_rx_fsm.vhd:45:23
  n71_o <= not din;
  -- work/uart_rx_fsm.vhd:45:16
  n73_o <= stav when n71_o = '0' else "001";
  -- work/uart_rx_fsm.vhd:44:13
  n75_o <= '1' when stav = "000" else '0';
  -- work/uart_rx_fsm.vhd:50:16
  n77_o <= stav when cnt_8 = '0' else "010";
  -- work/uart_rx_fsm.vhd:49:13
  n79_o <= '1' when stav = "001" else '0';
  -- work/uart_rx_fsm.vhd:56:16
  n81_o <= stav when bit_8 = '0' else "011";
  -- work/uart_rx_fsm.vhd:55:13
  n83_o <= '1' when stav = "010" else '0';
  -- work/uart_rx_fsm.vhd:62:16
  n85_o <= stav when n86_o = '0' else "100";
  -- work/uart_rx_fsm.vhd:62:16
  n86_o <= din and cnt_16;
  -- work/uart_rx_fsm.vhd:61:13
  n88_o <= '1' when stav = "011" else '0';
  -- work/uart_rx_fsm.vhd:69:13
  n90_o <= '1' when stav = "100" else '0';
  n91_o <= n90_o & n88_o & n83_o & n79_o & n75_o;
  -- work/uart_rx_fsm.vhd:41:10
  with n91_o select n94_o <=
    "000" when "10000",
    n85_o when "01000",
    n81_o when "00100",
    n77_o when "00010",
    n73_o when "00001",
    "XXX" when others;
  -- work/uart_rx_fsm.vhd:82:15
  n102_o <= '1' when stav = "000" else '0';
  -- work/uart_rx_fsm.vhd:86:18
  n104_o <= '1' when stav = "001" else '0';
  -- work/uart_rx_fsm.vhd:90:18
  n106_o <= '1' when stav = "010" else '0';
  -- work/uart_rx_fsm.vhd:94:18
  n108_o <= '1' when stav = "011" else '0';
  -- work/uart_rx_fsm.vhd:98:18
  n110_o <= '1' when stav = "100" else '0';
  -- work/uart_rx_fsm.vhd:98:7
  n113_o <= '0' when n110_o = '0' else '1';
  -- work/uart_rx_fsm.vhd:94:7
  n116_o <= '0' when n108_o = '0' else '1';
  -- work/uart_rx_fsm.vhd:94:7
  n118_o <= n113_o when n108_o = '0' else '0';
  -- work/uart_rx_fsm.vhd:90:7
  n120_o <= n116_o when n106_o = '0' else '1';
  -- work/uart_rx_fsm.vhd:90:7
  n123_o <= '0' when n106_o = '0' else '1';
  -- work/uart_rx_fsm.vhd:90:7
  n125_o <= n118_o when n106_o = '0' else '0';
  -- work/uart_rx_fsm.vhd:86:7
  n127_o <= n120_o when n104_o = '0' else '1';
  -- work/uart_rx_fsm.vhd:86:7
  n129_o <= n123_o when n104_o = '0' else '0';
  -- work/uart_rx_fsm.vhd:86:7
  n131_o <= n125_o when n104_o = '0' else '0';
  -- work/uart_rx_fsm.vhd:82:7
  n133_o <= n127_o when n102_o = '0' else '0';
  -- work/uart_rx_fsm.vhd:82:7
  n136_o <= n129_o when n102_o = '0' else '0';
  -- work/uart_rx_fsm.vhd:82:7
  n139_o <= n131_o when n102_o = '0' else '0';
  -- work/uart_rx_fsm.vhd:40:7
  process (clk, rst)
  begin
    if rst = '1' then
      n142_q <= "000";
    elsif rising_edge (clk) then
      n142_q <= n94_o;
    end if;
  end process;
end rtl;


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

architecture rtl of uart_rx is
  signal wrap_CLK: std_logic;
  signal wrap_RST: std_logic;
  signal wrap_DIN: std_logic;
  subtype typwrap_DOUT is std_logic_vector (7 downto 0);
  signal wrap_DOUT: typwrap_DOUT;
  signal wrap_DOUT_VLD: std_logic;
  signal cnt_8 : std_logic;
  signal cnt_16 : std_logic;
  signal bit_8 : std_logic;
  signal cnt_en : std_logic;
  signal rx_en : std_logic;
  signal clks : std_logic_vector (4 downto 0);
  signal bites : std_logic_vector (3 downto 0);
  signal accepted_bites : std_logic_vector (7 downto 0);
  signal fsm_c_cnt_en : std_logic;
  signal fsm_c_rx_en : std_logic;
  signal fsm_c_dout_vld : std_logic;
  signal n9_o : std_logic_vector (4 downto 0);
  signal n11_o : std_logic_vector (4 downto 0);
  signal n13_o : std_logic_vector (3 downto 0);
  signal n14_o : std_logic;
  signal n15_o : std_logic;
  signal n17_o : std_logic;
  signal n18_o : std_logic;
  signal n20_o : std_logic;
  signal n21_o : std_logic;
  signal n23_o : std_logic_vector (4 downto 0);
  signal n24_o : std_logic;
  signal n26_o : std_logic;
  signal n27_o : std_logic;
  signal n28_o : std_logic_vector (6 downto 0);
  signal n29_o : std_logic_vector (7 downto 0);
  signal n31_o : std_logic_vector (3 downto 0);
  signal n33_o : std_logic_vector (4 downto 0);
  signal n34_o : std_logic_vector (3 downto 0);
  signal n48_o : std_logic;
  signal n49_o : std_logic;
  signal n53_o : std_logic;
  signal n54_o : std_logic;
  signal n58_o : std_logic;
  signal n59_o : std_logic;
  signal n61_q : std_logic_vector (4 downto 0);
  signal n62_q : std_logic_vector (3 downto 0);
  signal n63_o : std_logic_vector (7 downto 0);
  signal n64_q : std_logic_vector (7 downto 0);
begin
  wrap_clk <= clk;
  wrap_rst <= rst;
  wrap_din <= din;
  dout <= wrap_dout;
  dout_vld <= wrap_dout_vld;
  wrap_DOUT <= accepted_bites;
  wrap_DOUT_VLD <= fsm_c_dout_vld;
  -- work/uart_rx.vhd:25:12
  cnt_8 <= n49_o; -- (signal)
  -- work/uart_rx.vhd:26:12
  cnt_16 <= n54_o; -- (signal)
  -- work/uart_rx.vhd:27:12
  bit_8 <= n59_o; -- (signal)
  -- work/uart_rx.vhd:28:12
  cnt_en <= fsm_c_cnt_en; -- (signal)
  -- work/uart_rx.vhd:29:12
  rx_en <= fsm_c_rx_en; -- (signal)
  -- work/uart_rx.vhd:31:12
  clks <= n61_q; -- (signal)
  -- work/uart_rx.vhd:32:12
  bites <= n62_q; -- (signal)
  -- work/uart_rx.vhd:33:12
  accepted_bites <= n64_q; -- (signal)
  -- work/uart_rx.vhd:38:5
  fsm : entity work.uart_rx_fsm port map (
    clk => wrap_CLK,
    rst => wrap_RST,
    din => wrap_DIN,
    cnt_8 => cnt_8,
    cnt_16 => cnt_16,
    bit_8 => bit_8,
    cnt_en => fsm_c_cnt_en,
    rx_en => fsm_c_rx_en,
    dout_vld => fsm_c_dout_vld);
  -- work/uart_rx.vhd:60:30
  n9_o <= std_logic_vector (unsigned (clks) + unsigned'("00001"));
  -- work/uart_rx.vhd:59:13
  n11_o <= "00000" when cnt_en = '0' else n9_o;
  -- work/uart_rx.vhd:59:13
  n13_o <= "0000" when cnt_en = '0' else bites;
  -- work/uart_rx.vhd:68:39
  n14_o <= not rx_en;
  -- work/uart_rx.vhd:68:29
  n15_o <= n14_o and cnt_en;
  -- work/uart_rx.vhd:68:54
  n17_o <= '1' when clks = "00111" else '0';
  -- work/uart_rx.vhd:68:45
  n18_o <= n17_o and n15_o;
  -- work/uart_rx.vhd:68:74
  n20_o <= '1' when bites = "0000" else '0';
  -- work/uart_rx.vhd:68:64
  n21_o <= n20_o and n18_o;
  -- work/uart_rx.vhd:68:13
  n23_o <= n11_o when n21_o = '0' else "00000";
  -- work/uart_rx.vhd:73:29
  n24_o <= rx_en and cnt_en;
  -- work/uart_rx.vhd:73:54
  n26_o <= '1' when clks = "01111" else '0';
  -- work/uart_rx.vhd:73:45
  n27_o <= n26_o and n24_o;
  -- work/uart_rx.vhd:77:55
  n28_o <= accepted_bites (7 downto 1);
  -- work/uart_rx.vhd:77:39
  n29_o <= wrap_DIN & n28_o;
  -- work/uart_rx.vhd:79:32
  n31_o <= std_logic_vector (unsigned (bites) + unsigned'("0001"));
  -- work/uart_rx.vhd:73:13
  n33_o <= n23_o when n27_o = '0' else "00000";
  -- work/uart_rx.vhd:73:13
  n34_o <= n13_o when n27_o = '0' else n31_o;
  -- work/uart_rx.vhd:85:29
  n48_o <= '1' when clks = "00111" else '0';
  -- work/uart_rx.vhd:85:19
  n49_o <= '0' when n48_o = '0' else '1';
  -- work/uart_rx.vhd:86:29
  n53_o <= '1' when clks = "01111" else '0';
  -- work/uart_rx.vhd:86:19
  n54_o <= '0' when n53_o = '0' else '1';
  -- work/uart_rx.vhd:87:30
  n58_o <= '1' when bites = "1000" else '0';
  -- work/uart_rx.vhd:87:19
  n59_o <= '0' when n58_o = '0' else '1';
  -- work/uart_rx.vhd:57:9
  process (wrap_CLK, wrap_RST)
  begin
    if wrap_RST = '1' then
      n61_q <= "00000";
    elsif rising_edge (wrap_CLK) then
      n61_q <= n33_o;
    end if;
  end process;
  -- work/uart_rx.vhd:57:9
  process (wrap_CLK, wrap_RST)
  begin
    if wrap_RST = '1' then
      n62_q <= "0000";
    elsif rising_edge (wrap_CLK) then
      n62_q <= n34_o;
    end if;
  end process;
  -- work/uart_rx.vhd:57:9
  n63_o <= accepted_bites when n27_o = '0' else n29_o;
  -- work/uart_rx.vhd:57:9
  process (wrap_CLK, wrap_RST)
  begin
    if wrap_RST = '1' then
      n64_q <= "00000000";
    elsif rising_edge (wrap_CLK) then
      n64_q <= n63_o;
    end if;
  end process;
end rtl;
