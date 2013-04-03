<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" exclude-result-prefixes="rdf foaf bio tudbio tud" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
		xmlns:dc="http://purl.org/dc/elements/1.1/"
		xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
		xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
		xmlns:foaf="http://xmlns.com/foaf/0.1/"
		xmlns:bio="http://purl.org/vocab/bio/0.1/"
		xmlns:tudbio="http://www.library.tudelft.nl/ns/rdf/bio/"
		xmlns:tud="http://www.tudelft.nl/ns/repo3/">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	<xsl:template match="/">
		<oai_dc:dc xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/ http://www.openarchives.org/OAI/2.0/oai_dc.xsd">
			<xsl:apply-templates/>
		</oai_dc:dc>
	</xsl:template>
	<xsl:template match="tud:person">
		<dc:title><xsl:value-of select="normalize-space(tud:name)"/></dc:title>
		<dc:description>
			<xsl:variable name="text">
				<xsl:text>Person</xsl:text>
				<xsl:for-each select="tud:birth/tud:date">
					<xsl:variable name="date" select="normalize-space(.)"/>
					<xsl:if test="$date!=''">, born <xsl:value-of select="$date"/></xsl:if>
				</xsl:for-each>
				<xsl:for-each select="tud:death/tud:date">
					<xsl:variable name="date" select="normalize-space(.)"/>
					<xsl:if test="$date!=''">, died <xsl:value-of select="$date"/></xsl:if>
				</xsl:for-each>
				<xsl:text>. </xsl:text>
				<xsl:for-each select="tud:description"><xsl:value-of select="."/></xsl:for-each>
			</xsl:variable>
			<xsl:value-of select="normalize-space($text)"/>
		</dc:description>
	</xsl:template>
	<xsl:template match="tud:organization">
		<dc:title><xsl:value-of select="normalize-space(tud:name)"/></dc:title>
		<xsl:for-each select="tud:description"><dc:description><xsl:value-of select="normalize-space(.)"/></dc:description></xsl:for-each>
	</xsl:template>
</xsl:stylesheet>
