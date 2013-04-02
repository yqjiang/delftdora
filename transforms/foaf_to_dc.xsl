<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" exclude-result-prefixes="rdf foaf bio tudbio" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
		xmlns:dc="http://purl.org/dc/elements/1.1/"
		xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
		xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
		xmlns:foaf="http://xmlns.com/foaf/0.1/"
		xmlns:bio="http://purl.org/vocab/bio/0.1/"
		xmlns:tudbio="http://www.library.tudelft.nl/ns/rdf/bio/">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	<xsl:template match="rdf:RDF">
		<oai_dc:dc xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/ http://www.openarchives.org/OAI/2.0/oai_dc.xsd">
			<xsl:apply-templates/>
		</oai_dc:dc>
	</xsl:template>
	<xsl:template match="foaf:Person">
		<dc:title><xsl:value-of select="normalize-space(foaf:name)"/></dc:title>
		<dc:description>
			<xsl:variable name="text">
				<xsl:text>Person.</xsl:text>
				<xsl:for-each select="tudbio:birth[*]"> Born <xsl:call-template name="event"/></xsl:for-each>
				<xsl:for-each select="tudbio:death[*]"> Died <xsl:call-template name="event"/></xsl:for-each>
				<xsl:for-each select="tudbio:nationality[normalize-space(.) !='']"> Nationality: <xsl:value-of select="."/>.</xsl:for-each>
				<xsl:for-each select="tudbio:education[normalize-space(.) !='']"> Education: <xsl:value-of select="."/>.</xsl:for-each>
			</xsl:variable>
			<xsl:value-of select="normalize-space($text)"/>
		</dc:description>
		<xsl:for-each select="foaf:primaryTopicOf/@rdf:resource[normalize-space(.) !='']">
			<dc:relation><xsl:value-of select="normalize-space(.)"/></dc:relation>
		</xsl:for-each>
	</xsl:template>
	<xsl:template name="event"><xsl:value-of select="(bio:date)"/><xsl:for-each select="bio:place/foaf:name[normalize-space(.) !='']"> in <xsl:value-of select="."/></xsl:for-each>.</xsl:template>
</xsl:stylesheet>
