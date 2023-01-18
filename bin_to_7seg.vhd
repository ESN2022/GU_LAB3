library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity bin_to_7seg is
    Port ( B : in  STD_LOGIC_VECTOR (3 downto 0);
           S : out  STD_LOGIC_VECTOR (7 downto 0));
end bin_to_7seg;

architecture rtl of bin_to_7seg is
begin
    process(B)
    begin
        case B is
            when "0000" => S <= "11000000";
            when "0001" => S <= "11111001";
            when "0010" => S <= "10100100";
            when "0011" => S <= "10110000";
            when "0100" => S <= "10011001";
            when "0101" => S <= "10010010";
            when "0110" => S <= "10000010";
            when "0111" => S <= "11111000";
            when "1000" => S <= "10000000";
            when "1001" => S <= "10010000";
            when others => S <= "11111110";
        end case;
    end process;
end architecture;