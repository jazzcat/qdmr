<?xml version="1.0"?>

<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:import href="file:///usr/share/xml/docbook/stylesheet/docbook-xsl/fo/docbook.xsl"/>

  <xsl:attribute-set name="monospace.verbatim.properties">
    <!--<xsl:attribute name="font-family">Lucida Sans Typewriter</xsl:attribute>//-->
    <xsl:attribute name="font-size">9pt</xsl:attribute>
    <xsl:attribute name="keep-together.within-column">always</xsl:attribute>
  </xsl:attribute-set>

  <xsl:param name="paper.type" select="'A4'"/>
  <xsl:param name="highlight.source" select="1"/>
</xsl:stylesheet>
